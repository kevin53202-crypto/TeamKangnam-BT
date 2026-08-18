#include "Task_WEZAttack.h"

namespace Action
{
	PortsList Task_WEZAttack::providedPorts()
	{
		return {
				InputPort<CPPBlackBoard*>("BB")
		};
	}

	NodeStatus Task_WEZAttack::tick()
	{
		Optional<CPPBlackBoard*> BB = getInput<CPPBlackBoard*>("BB");

		Vector3 toTarget = (*BB)->TargetLocaion_Cartesian - (*BB)->MyLocation_Cartesian;
		double dist = toTarget.length();
		Vector3 losNow = (dist > 1e-3) ? (toTarget / dist) : Vector3(1.0, 0.0, 0.0);

		double dt = (*BB)->DeltaSecond;
		Vector3 aimDir = losNow;

		if (m_hasPrevLos && dt > 1e-4)
		{
			// LOS(조준선) 방향 벡터의 초당 변화율을 각속도로 근사하고, 그 회전을
			// 앞서 예측한 방향을 조준점으로 삼는다. 순수 추적(리드 0)은
			// 상대가 선회 중일 때 pursuit-curve lag으로 조준각이 일정 오프셋에서
			// 안 좁혀지는 문제(실측: 최소 own_ata 26도)가 있었는데, 이 방식은
			// 상대의 회전을 따라가며 그 오프셋을 줄이는 걸 목표로 한다.
			//
			// 2026-08-17 (좌표 버그 수정 후 재튜닝): 고정 0.35초 리드로는 헤드온
			// 머지처럼 짧게 스쳐지나가는 패스에서 13도 정도까지밖에 안 좁혀짐
			// (실측). 아직 많이 안 맞았을 때(Los_Degree 큼)는 리드를 더 크게 줘서
			// 상대 선회를 더 적극적으로 따라잡고, 거의 다 맞았을 때(Los_Degree 작음)는
			// 리드를 줄여 오버슛 없이 정밀 추종하도록 Los_Degree에 비례해 조절.
			double losDeg = (*BB)->Los_Degree;
			double lookaheadSec = losDeg / 30.0;
			lookaheadSec = lookaheadSec < 0.2 ? 0.2 : (lookaheadSec > 1.2 ? 1.2 : lookaheadSec);

			Vector3 losRate = (losNow - m_prevLos) / dt;
			Vector3 predicted = losNow + losRate * lookaheadSec;
			double predictedLen = predicted.length();
			if (predictedLen > 1e-3)
			{
				aimDir = predicted / predictedLen;
			}
		}

		m_prevLos = losNow;
		m_hasPrevLos = true;

		(*BB)->VP_Cartesian = (*BB)->MyLocation_Cartesian + aimDir * 1000.0;
		(*BB)->Throttle = 1.0f;

		return NodeStatus::SUCCESS;
	}
}
