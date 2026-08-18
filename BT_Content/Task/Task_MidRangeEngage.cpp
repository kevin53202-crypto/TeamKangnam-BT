#include "Task_MidRangeEngage.h"

namespace Action
{
	PortsList Task_MidRangeEngage::providedPorts()
	{
		return {
				InputPort<CPPBlackBoard*>("BB")
		};
	}

	NodeStatus Task_MidRangeEngage::tick()
	{
		Optional<CPPBlackBoard*> BB = getInput<CPPBlackBoard*>("BB");

		// 2026-08-16 (3차 튜닝): closure-time 리드로 목표 앞을 가로지르는 궤적은
		// 실측 결과 WEZ 사거리 체류가 0.6%밖에 안 나왔다(스쳐 지나가기만 함) --
		// 대신 상대의 "6시 방향"(꼬리)을 조준점으로 삼아 선회해 들어가는
		// stern conversion(후미 전환) 기동으로 교체. 목표와 같은 진행 방향/속도로
		// 붙는 궤적이라 WEZAttack 인수인계 시점에 상대 속도가 낮고(체류시간 유리),
		// 상대에게 진 AA도 자연히 좋아진다(꼬리를 잡으러 가는 형태이므로).
		// trail 거리는 거리 비례로 줄여서 914m WEZ 경계에서 자연스럽게 순수 추적에 가까워짐.
		float trailDistance = (float)((*BB)->Distance * 0.4);
		trailDistance = trailDistance < 150.0f ? 150.0f : (trailDistance > 900.0f ? 900.0f : trailDistance);

		// 2026-08-17: 고도유지 보정을 두 강도로 시도했으나 60개 시나리오 배치 검증
		// 결과 둘 다 개선 효과가 없었음(3000m/0.5 -> 무교전 폭증 75%, 1200m/0.15 ->
		// 기준선과 오차범위 내 동일, alpha=90은 오히려 소폭 악화). 자멸 문제는 VP
		// 조준점 보정 수준으로는 해결 안 되는 것으로 판단해 원래(고도유지 보정 없는)
		// 버전으로 되돌림 -- 자세한 근거는 md/종합_판단_20260817.md 참고.
		(*BB)->VP_Cartesian = (*BB)->TargetLocaion_Cartesian - (*BB)->TargetForwardVector * trailDistance;

		(*BB)->Throttle = 1.0f;

		return NodeStatus::SUCCESS;
	}
}
