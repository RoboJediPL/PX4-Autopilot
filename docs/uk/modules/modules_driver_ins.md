# Modules Reference: Ins (Driver)

## vectornav

Source: [drivers/ins/vectornav](https://github.com/PX4/PX4-Autopilot/tree/main/src/drivers/ins/vectornav)

### Опис

Драйвер послідовної шини для VectorNav VN-100, VN-200, VN-300.

Більшість плат налаштовано на увімкнення/запуск драйвера на вказаному UART за допомогою параметра SENS_VN_CFG.

Інформація про налаштування/використання: https://docs.px4.io/main/en/sensor/vectornav.html

### Приклади

Attempt to start driver on a specified serial device.

```
vectornav start -d /dev/ttyS1
```

Stop driver

```
vectornav stop
```

### Usage {#vectornav_usage}

```
vectornav <command> [arguments...]
 Commands:
   start         Start driver
     -d <val>    Serial device

   status        Driver status

   stop          Stop driver

   status        Print driver status
```
