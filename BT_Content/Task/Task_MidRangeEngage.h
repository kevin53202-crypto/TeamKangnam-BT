#pragma once
#include "../../behaviortree_cpp_v3/action_node.h"
#include "../../behaviortree_cpp_v3/bt_factory.h"
#include "../../../Geometry/Vector3.h"
#include "../Functions.h"
#include "../BlackBoard/CPPBlackBoard.h"

using namespace BT;

namespace Action
{
	// 2026-08-16: 중거리 추격(914m < Distance <= 3000m) 구간. 절반 리드 펄슛으로
	// 선회를 잘라들어가(cut the circle) AA를 줄이면서 WEZ 진입을 준비한다.
	class Task_MidRangeEngage : public SyncActionNode
	{
	public:
		Task_MidRangeEngage(const std::string& name, const NodeConfiguration& config) : SyncActionNode(name, config)
		{
		}

		~Task_MidRangeEngage()
		{
		}

		static PortsList providedPorts();
		NodeStatus tick() override;
	};
}
