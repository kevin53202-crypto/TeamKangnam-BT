# TeamKangnam BT

AI Pilot Top-Gun Challenge — TeamKangnam 전술 행동 트리(Behavior Tree) 구현.

- `Rule_team01.xml`: 6단계 우선순위 Fallback 전술 트리 (비상 고도 회복 → WEZ 근거리 사격 → 위협 회피 → 중거리 교전 → 원거리 접근 → 기본 유지)
- `BT_Content/Task/`: 각 전술 단계별 기동 로직 (WEZAttack, MidRangeEngage, LongRangeApproach, Evade, EmergencyPullUp 등)
- `BT_Content/Decorator/`: 조건 판단 노드 (거리/고도/위협 임계값 체크)
- `BT_Content/Service/`: 매 틱 전술 블랙보드 갱신 (표적 선정, 거리/각도 계산 등)
- `BT_Content/BlackBoard/`: 실시간 교전 상태 공유 구조체
