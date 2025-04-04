#include "LoRaBoards.h"

#include "App.h"

void setup()
{
    setupBoards(true);

    app.setup();
}

void loop()
{
    app.loop();
}