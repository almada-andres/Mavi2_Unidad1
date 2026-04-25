#ifndef NDEBUG
#include <vld.h>
#endif
#include "raylib.h"
#include <box2d.h>
#include <vector>

struct PhysicsBox
{
    b2Body* body;
    float width;
    float height;
    Color color;
};

struct PhysicsCircle
{
    b2Body* body;
    float radius;
    Color color;
};

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "MAVI II - Mavix Despierta");
    SetTargetFPS(60);

    Color fondo = { 110, 100, 215, 255 };
    Color textoPrincipal = RAYWHITE;
    Color textoSecundario = DARKPURPLE;
    Color sueloColor = Fade(DARKGREEN, 0.7f);

    // Mundo físico
    b2Vec2 gravity(0.0f, 9.8f);
    b2World world(gravity);

    // -----------------------------
    // Suelo estático
    // -----------------------------
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(screenWidth / 2.0f, screenHeight - 40.0f);
    b2Body* groundBody = world.CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(screenWidth / 2.0f, 20.0f);
    groundBody->CreateFixture(&groundShape, 0.0f);

    std::vector<PhysicsBox> boxes;
    std::vector<PhysicsCircle> circles;

    // Variables para cajas
    float currentAngle = 0.0f; // Angulo para la nueva caja
    const float rotationSpeed = 2.0f; // Velocidad de rotacion

    while (!WindowShouldClose())
    {
        // Input

        // Para rotacion previa
        if (IsKeyDown(KEY_LEFT)) currentAngle -= rotationSpeed * GetFrameTime();
        if (IsKeyDown(KEY_RIGHT)) currentAngle += rotationSpeed * GetFrameTime();

        // Creacion de caja al presionar ESPACIO
        if (IsKeyPressed(KEY_SPACE))
        {
            b2BodyDef boxDef;
            boxDef.type = b2_dynamicBody;
            // Posicion de "aparicion" de cajas
            boxDef.position.Set(screenWidth / 2.0f, 50.0f);
            // Aplicacion del angulo
            boxDef.angle = currentAngle;

            b2Body* boxBody = world.CreateBody(&boxDef);

            b2PolygonShape boxShape;
            boxShape.SetAsBox(25.0f, 25.0f);

            b2FixtureDef boxFixture;
            boxFixture.shape = &boxShape;
            boxFixture.density = 1.0f;
            boxFixture.friction = 0.3f;
            boxFixture.restitution = 0.2f;

            boxBody->CreateFixture(&boxFixture);

            // Guardado en el vector para renderizar
            boxes.push_back({ boxBody, 50.0f, 50.0f, Fade(GOLD, 0.9f) });
        }

        // Actualizacion
        world.Step(1.0f / 60.0f, 8, 3);

        // DIbujado
        BeginDrawing();
        ClearBackground(fondo);

        // Suelo visual
        DrawRectangle(0, screenHeight - 60, screenWidth, 40, sueloColor);

        for (const auto& box : boxes)
        {
            b2Vec2 pos = box.body->GetPosition();
            float angle = box.body->GetAngle() * RAD2DEG;

            Rectangle rect = { pos.x, pos.y, box.width, box.height };
            Vector2 origin = { box.width / 2.0f, box.height / 2.0f };

            // parte maciza de la caja
            DrawRectanglePro(rect, origin, angle, box.color);

            // Contorno de la caja
            DrawPolyLinesEx({ pos.x, pos.y }, 4, box.width / 1.4142f, angle + 45, 2, DARKBLUE);
        }

        // Instrucciones
        DrawRectangle(0, 0, screenWidth, 110, Fade(BLACK, 0.3f));
        DrawText("Mavix: Exploracion Fisica", 20, 20, 30, RAYWHITE);
        DrawText("ESPACIO: Crear Caja | FLECHAS: Rotar Angulo Inicial", 20, 60, 20, LIGHTGRAY);

        // Indicador de angulo
        DrawText(TextFormat("Angulo actual: %.2f", currentAngle * RAD2DEG), 750, 60, 20, GOLD);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}