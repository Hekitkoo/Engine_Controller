# Engine_Controller
Используемый цифровой драйвер:
![dm542](docs/DM542.png "dm542")
Используемая плата arduino:
![arduino leonardo](docs/leonardo.png "arduino leonardo")
##### В случае если повредили провод (как переобжать)
![Распиновка](docs/RJ-45%20pins.bmp "Распиновка")
##### Назначение RJ-45
###### Со стороны arduino
1. бело-оранжевый: 13pin digital (к DIR+)
2. оранжевый: 12pin digital (к ENA+)
3. бело-зелёный: питание+
4. синий: (земля) ground pins шина
5. бело-синий: 10pin digital (к PUL+)
6. зелёный: питание-
7. бело-коричневый: не используется
8. коричневый: не используется
###### Со стороны станка
1. бело-оранжевый: DIR+ (к 13pin digital)
2. оранжевый: ENA+ (к 12pin digital)
3. бело-зелёный: питание+
4. синий: (земля) ground pins шина
5. бело-синий: PUL+ (к 10pin digital)
6. зелёный: питание-
7. бело-коричневый: не используется
8. коричневый: не используется

##### Цепь
![Цепь](docs/Wiring%20Components.png "Цепь")
##### Схема .pdf
            Схема ↓
Схема > ![Схема](docs/Wiring%20Components.pdf "Схема") < Схема <br/>
                    *Схема ↑
##### Как примерно будет выглядить пульт
![controller_v1.jpg](docs/controller_v1.jpg "controller_v1.jpg")