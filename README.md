# Транспортный справочник 
## Описание
Реализована система для управления и обработки данных о городских маршрутах и остановках. Проект поддерживает добавление данных о маршрутах автобусов и остановках, вычисление оптимальных маршрутов и визуализацию транспортной сети в формате SVG.

Основные возможности:
- Добавление автобусов и остановок.
- Вычисление маршрутов между остановками с учётом времени ожидания и скорости автобусов.
- Визуализация карты маршрутов с использованием SVG.
## Пример
<details>

<summary>Пример 1. Входной JSON-документ. </summary>

```json
{
    "base_requests": [
        {
            "is_roundtrip": true,
            "name": "297",
            "stops": [
                "Biryulyovo Zapadnoye",
                "Biryulyovo Tovarnaya",
                "Universam",
                "Biryulyovo Zapadnoye" 
            ], 
            "type": "Bus"
        },
        {
            "is_roundtrip": false,
            "name": "635",
            "stops": [
                "Biryulyovo Tovarnaya",
                "Universam",
                "Prazhskaya"
            ], 
            "type": "Bus"
        },
        {
            "is_roundtrip": true,
            "name": "750",
            "stops": [
                "Marino",
                "Prazhskaya",
                "Ochakovo",
                "Marino"
            ], 
            "type": "Bus"
        },
        {
            "is_roundtrip": false,
            "name": "850",
            "stops": [
                "Universam",
                "Kuntsevo",
                "Ochakovo"
            ], 
            "type": "Bus"
        },
        {
            "is_roundtrip": true,
            "name": "123",
            "stops": [
                "Sakura Park",
                "Rose Garden",
                "Tulip Square"
            ], 
            "type": "Bus"
        },
        {
            "is_roundtrip": true,
            "name": "55",
            "stops": [
                "South Gate",
                "City Mall",
                "MKAD",
                "Sadovod",
                "Ryazan Avenue",
                "Biryulyovo Zapadnoye"
            ], 
            "type": "Bus"
        },
        {
            "is_roundtrip": true,
            "name": "114",
            "stops": [
                "Northern Station",
                "Green Park",
                "Rose Garden",
                "Sakura Park",
                "Northern Station"
            ], 
            "type": "Bus"
        },
        {
            "latitude": 55.574371,
            "longitude": 37.6517,
            "name": "Biryulyovo Zapadnoye",
            "road_distances": {
                "Biryulyovo Tovarnaya": 2600
            },
            "type": "Stop"
        },
        {
            "latitude": 55.587655,
            "longitude": 37.645687,
            "name": "Universam",
            "road_distances": {
                "Biryulyovo Tovarnaya": 1380,
                "Biryulyovo Zapadnoye": 2500,
                "Prazhskaya": 4650
            },
            "type": "Stop"
        },
        {
            "latitude": 55.592028,
            "longitude": 37.653656,
            "name": "Biryulyovo Tovarnaya",
            "road_distances": {
                "Universam": 890
            },
            "type": "Stop"
        },
        {
            "latitude": 55.611717,
            "longitude": 37.603938,
            "name": "Prazhskaya",
            "road_distances": {
                "Universam": 4650,
                "Ochakovo": 3000
            },
            "type": "Stop"
        },
        {
            "latitude": 55.614000,
            "longitude": 37.651000,
            "name": "Kuntsevo",
            "road_distances": {
                "Universam": 2000,
                "Ochakovo": 1800
            },
            "type": "Stop"
        },
        {
            "latitude": 55.610000,
            "longitude": 37.659000,
            "name": "Ochakovo",
            "road_distances": {
                "Marino": 2000,
                "Prazhskaya": 3000
            },
            "type": "Stop"
        },
        {
            "latitude": 55.607000,
            "longitude": 37.640000,
            "name": "Marino",
            "road_distances": {
                "Ochakovo": 2000
            },
            "type": "Stop"
        },
        {
            "latitude": 55.602000,
            "longitude": 37.645000,
            "name": "Sakura Park",
            "road_distances": {
                "Universam": 1300
            },
            "type": "Stop"
        },
        {
            "latitude": 55.604500,
            "longitude": 37.652000,
            "name": "Rose Garden",
            "road_distances": {
                "Sakura Park": 900,
                "Tulip Square": 1200
            },
            "type": "Stop"
        },
        {
            "latitude": 55.605800,
            "longitude": 37.650500,
            "name": "Tulip Square",
            "road_distances": {
                "Rose Garden": 1200
            },
            "type": "Stop"
        },
        {
            "latitude": 55.621000,
            "longitude": 37.54,
            "name": "South Gate",
            "road_distances": {
                "MKAD" : 2000
            },
            "type": "Stop"
        },
        {
            "latitude": 55.60,
            "longitude": 37.6217,
            "name": "MKAD",
            "road_distances": {
                "Sadovod": 1300
            },
            "type": "Stop"
        },
        {
            "latitude": 55.60,
            "longitude": 37.6317,
            "name": "Sadovod",
            "road_distances": {
                "Ryazan Avenue": 1000
            },
            "type": "Stop"
        },
        {
            "latitude": 55.59,
            "longitude": 37.6217,
            "name": "Ryazan Avenue",
            "road_distances": {
                "Biryulyovo Zapadnoye": 850
            },
            "type": "Stop"
        },
        {
            "latitude": 55.630000,
            "longitude": 37.540000,
            "name": "City Mall",
            "road_distances": {
                "South Gate": 1200,
                "MKAD": 1800
            },
            "type": "Stop"
        },
        {
            "latitude": 55.620000,
            "longitude": 37.670000,
            "name": "Green Park",
            "road_distances": {
                "Tulip Square": 1100,
                "Rose Garden": 1300
            },
            "type": "Stop"
        },
        {
            "latitude": 55.640000,
            "longitude": 37.620000,
            "name": "Northern Station",
            "road_distances": {
                "Ryazan Avenue": 2400,
                "Sadovod": 2100
            },
            "type": "Stop"
        }
        
    ],
    "render_settings": {
        "bus_label_font_size": 20,
        "bus_label_offset": [
            7,
            15
        ],
        "color_palette": [
            [
                237,
                237,
                237
            ],
            [
                255, 
                197, 
                251
            ],
            [
                214, 
                197, 
                247
            ],
            [
                198, 
                247, 
                197
            ], 
            [
                247,
                238,
                197
            ],
            [
                158, 
                182, 
                255
            ],
            [
                224,
                255,
                255
            ],
            [
                255, 
                46, 
                46
            ],
            [
                175,
                255, 
                195
            ]
        ],
        "height": 576,
        "line_width": 14,
        "padding": 30,
        "stop_label_font_size": 20,
        "stop_label_offset": [
            7,
            -3
        ],
        "stop_radius": 5,
        "underlayer_color": [
            255,
            255,
            255,
            0.85
        ],
        "underlayer_width": 3,
        "width": 1251
    },
    "routing_settings": {
        "bus_velocity": 40,
        "bus_wait_time": 6
    },
    "stat_requests": [
        {
            "id": 1,
            "name": "297",
            "type": "Bus"
        },
        {
            "id": 2,
            "name": "635",
            "type": "Bus"
        },
        {
            "id": 3,
            "name": "Universam",
            "type": "Stop"
        },
        {
            "from": "Biryulyovo Zapadnoye",
            "id": 4,
            "to": "Universam",
            "type": "Route"
        },
        {
            "from": "Biryulyovo Zapadnoye",
            "id": 5,
            "to": "Prazhskaya",
            "type": "Route"
        },
        {
            "id": 6,
            "name": "750",
            "type": "Bus"
        },
        {
            "id": 7,
            "name": "850",
            "type": "Bus"
        },
        {
            "id": 8,
            "name": "123",
            "type": "Bus"
        }
    ]
}
```
</details>
<details>

<summary>Пример 1. Выходной JSON-документ.</summary>

```json
[
    {
        "curvature": 1.42963,
        "request_id": 1,
        "route_length": 5990,
        "stop_count": 4,
        "unique_stop_count": 3
    },
    {
        "curvature": 1.30156,
        "request_id": 2,
        "route_length": 11570,
        "stop_count": 5,
        "unique_stop_count": 3
    },
    {
        "buses": [
            "297",
            "635",
            "850"
        ],
        "request_id": 3
    },
    {
        "items": [
            {
                "stop_name": "Biryulyovo Zapadnoye",
                "time": 6,
                "type": "Wait"
            },
            {
                "bus": "297",
                "span_count": 2,
                "time": 5.235,
                "type": "Bus"
            }
        ],
        "request_id": 4,
        "total_time": 11.235
    },
    {
        "items": [
            {
                "stop_name": "Biryulyovo Zapadnoye",
                "time": 6,
                "type": "Wait"
            },
            {
                "bus": "297",
                "span_count": 1,
                "time": 3.9,
                "type": "Bus"
            },
            {
                "stop_name": "Biryulyovo Tovarnaya",
                "time": 6,
                "type": "Wait"
            },
            {
                "bus": "635",
                "span_count": 2,
                "time": 8.31,
                "type": "Bus"
            }
        ],
        "request_id": 5,
        "total_time": 24.21
    },
    {
        "curvature": 0.711506,
        "request_id": 6,
        "route_length": 5000,
        "stop_count": 4,
        "unique_stop_count": 3
    },
    {
        "curvature": 1.0499,
        "request_id": 7,
        "route_length": 7600,
        "stop_count": 5,
        "unique_stop_count": 3
    },
    {
        "curvature": 3.03133,
        "request_id": 8,
        "route_length": 2100,
        "stop_count": 3,
        "unique_stop_count": 3
    }
]
```
</details>
<details>

<summary>Пример 1. Визуализация с помощью SVG.</summary>

![image](https://github.com/user-attachments/assets/c0170318-ebfd-48fc-ac09-c137291a88f3)

</details>

## Сборка и запуск
```bash
git clone git@github.com:burakshaevn/cpp-transport-catalogue.git
mkdir build
cd build
cmake ../transport-catalogue
cmake --build .
```
1. После сборки по адресу `../cpp-transport-catalogue/build/Debug` появится TransportCatalogue.exe
2. Запусаем и указываем JSON-документ из примера 1.
3. После успешного выполнения в терминал будет выведен выходной JSON-документ.
4. По адресу `../cpp-transport-catalogue/` появится файл map.svg с рендером.

## Формат входных данных
Входные данные принимаются в формате JSON и содержат четыре основные категории:
- `base_requests` — запросы на добавление информации о маршрутах и остановках;
- `render_settings` — параметры для визуализации карты;
- `routing_settings` — настройки для расчёта маршрутов;
- `stat_requests` — запросы на получение статистики по маршрутам и остановкам.
```json
{
  "base_requests": [ ... ],
  "render_settings": { ... },
  "routing_settings": { ... }, 
  "stat_requests": [ ... ]
}
```
### Массив `base_requests`
Содержит запросы на добавление в базу данных в формате:

Добавление автобуса (то есть, type = "Bus"):
```json
{
    "is_roundtrip": <круговой ли маршрут>
    "name": <название маршрута>,
    "stops": [
        <список остановок, через которые проходит автобус>
    ], 
    "type": <тип запроса (в этом случае "Bus")>
}
```
Добавление остановки (то есть, type = "Stop"):
```json
{
    "latitude": <широта остановки>,
    "longitude": <долгота остановки>,
    "name": <название остановки>,
    "road_distances": {
        <словарь с расстояними до других остановок, идущих до/после остановки по маршруту>
    },
    "type": <тип запроса (в этом случае "Stop")>
}
```
### Словарь `render_settings`
```json
{
    "bus_label_font_size": <размер шрифта для подписей маршрутов>,
    "bus_label_offset": [
        <смещение по оси X для подписей маршрутов>,
        <смещение по оси Y для подписей маршрутов>
    ],
    "color_palette": [
        [
            <красный компонент цвета>,
            <зеленый компонент цвета>,
            <синий компонент цвета>
        ]
    ],
    "height": <высота карты>,
    "line_width": <ширина линий маршрутов>,
    "padding": <внешний отступ для карты>,
    "stop_label_font_size": <размер шрифта для подписей остановок>,
    "stop_label_offset": [
        <смещение по оси X для подписей остановок>,
        <смещение по оси Y для подписей остановок>
    ],
    "stop_radius": <радиус остановок>,
    "underlayer_color": [
        <красный компонент подложки>,
        <зеленый компонент подложки>,
        <синий компонент подложки>,
        <прозрачность подложки>
    ],
    "underlayer_width": <ширина подложки>,
    "width": <ширина карты>
}
```
### Словарь `routing_settings`
Содержит настройки маршрутизации, такие как скорость автобуса и время ожидания на остановках.
```json
    "routing_settings": {
        "bus_velocity": <скорость автобуса (км/ч)>,
        "bus_wait_time": <время ожидания на остановке (мин)>
    }
```
### Массив `stat_requests`
Содержит запросы для получения информации о маршрутах, автобусах и остановках, включая конкретные запросы на информацию о маршрутах между остановками.
```json
    "stat_requests": [
        {
            "id": <идентификатор запроса>,
            "name": <название маршрута или остановки>,
            "type": <тип запроса (Bus/Stop)>
        },
        {
            "from": <название начальной остановки>,
            "id": <идентификатор запроса>,
            "to": <название конечной остановки>,
            "type": "Route"
        },
        ...
    ]
}
```

## Основные классы
#### TransportCatalogue
Класс для хранения информации об автобусных маршрутах и остановках. Он поддерживает добавление данных о маршрутах, остановках и расстояниях между ними, а также предоставляет методы для получения информации по этим данным.
#### MapRenderer
Отвечает за визуализацию карты автобусных маршрутов в формате SVG. Используя настройки рендеринга, он строит схему маршрутов с остановками, линиями и текстом.
#### RequestHandler
Класс, отвечающий за обработку запросов от пользователя к базе данных каталога. Он получает запросы на основе JSON-структуры и взаимодействует с TransportCatalogue для получения нужной информации, например, о маршрутах или остановках.
#### Router
Отвечает за нахождение кратчайших маршрутов между остановками с учетом времени ожидания и скорости движения автобусов. Использует алгоритмы поиска кратчайших путей для предоставления пользователю маршрутов.
