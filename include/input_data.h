#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#define MSEC_IN_SEC 1000                    /* Количество милисекунд в секунде */
#define MSEC_IN_MIN 60000                   /* Количество милисекунд в минуте */
#define TRACK_COUNT 500                     /* Количество дорожек */
#define SURFACE_COUNT 4                     /* Количество поверхностей */
#define SECTORS_ON_HEAD_COUNT 16            /* Количество секторов на поверхности */
#define HEAD_TRANSITION_TIME 0.5            /* Время перехода головки между двумя дорожками (мс) */
#define DISK_ROTATION_SPEED 10000           /* Скорость вращения диска (об/мин) */
#define MAX_INTERVAL_TIME (2 * MSEC_IN_SEC) /* Максимальный интервал между запросами (мс) */
#define MAX_CONSECUTIVE_SECTORS 16          /* Максимальное количество последовательных обрабатываемых секторов за запрос */
#define SIMULATE_TIME (5 * MSEC_IN_MIN)     /* Интервал модельного времени (мс) */

#endif