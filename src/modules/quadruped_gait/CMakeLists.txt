px4_add_module(
    MODULE modules__quadruped_gait
    MAIN quadruped_gait
    SRCS
        QuadrupedGait.cpp
    DEPENDS
        px4_work_queue
    MODULE_CONFIG
        module.yaml
)
