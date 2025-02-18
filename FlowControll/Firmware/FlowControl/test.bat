echo off

ceedling clobber|more
cls


if %1.==. (
    ceedling test:test_bsp
    ceedling test:test_buffer8
    ceedling test:test_buffer16
    ceedling test:test_buffer_c
    ceedling test:test_command
    ceedling test:test_DRV8823
    ceedling test:test_motioncontrol
    ceedling test:test_LED
    ceedling test:test_relay
) else (
    ceedling test:test_%*
)

