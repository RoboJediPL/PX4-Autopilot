uint64 timestamp # time since system start (microseconds)

float32[4] turn_setpoints   # [-1, 1] Turn Motor (TM) setpoints for each leg [LF, RF, LR, RR]
float32[4] wheel_setpoints  # [-1, 1] Spin Motor (SM) setpoints [-1: backward, 1: forward]
float32[4] rotate_setpoints # [-1, 1] Rotate Motor (RM) setpoints
float32[4] pulley_setpoints # [-1, 1] Pulley Motor (PM) setpoints
