#define _TASK_MICRO_RES
#include <TaskScheduler.h>

#define LED D5
#define DIR1 D2
#define DIR2 D4
#define STEP D1
#define ENABLE D0

enum ESTADOS
{
    PARADO,
    ACELERANDO,
    CRUCERO,
    FRENANDO
};

Scheduler runner;
void GeneradorDePulsos();

float velocidad = 25;
int estado = PARADO;
bool girando = 0;

// Tasks
Task GenPasos(100, -1, &GeneradorDePulsos);

// se ejecuta cada 100 us
void GeneradorDePulsos()
{
    if (estado == ACELERANDO)
    {
        if (velocidad > 60)
            velocidad -= 0.08;
        else if (velocidad > 50)
            velocidad -= 0.005;
        else if (velocidad > 35 && !girando)
            velocidad -= 0.007;
        else if (velocidad > 25 && !girando)
            velocidad -= 0.01;
        else
        {
            estado = CRUCERO;
            Serial.println("crucero");
            digitalWrite(LED, 1);
        }
    }
    else if (estado == FRENANDO)
    {
        if (velocidad < 60)
        {
            velocidad += 0.01;
        }
        else
        {
            Serial.println("parado");
            estado = PARADO;
            GenPasos.disable();
            digitalWrite(ENABLE, 1);
            digitalWrite(LED, 0);
        }
    }

    digitalWrite(STEP, LOW);
    static int contador = 0;
    if (contador++ < velocidad)
        return;
    contador = 0;
    digitalWrite(STEP, HIGH);
}

void FrenarYArrancar()
{
    if (estado != PARADO)
    {
        estado = FRENANDO;
        while (estado != PARADO)
        {
            runner.execute();
        }
    }
    velocidad = 65;
    estado = ACELERANDO;
}

void Frenar()
{
    estado = FRENANDO;
    Serial.println("parando");
}

void Parar()
{
    estado = PARADO;
    digitalWrite(ENABLE, 1);
    GenPasos.disable();
    Serial.println("stop");
}

void Avanzar()
{
    girando = 0;
    FrenarYArrancar();
    digitalWrite(DIR1, 0);
    digitalWrite(DIR2, 0);
    digitalWrite(ENABLE, 0);
    GenPasos.enable();
    Serial.println("arrancando");
}

void Retroceder()
{
    girando = 0;
    FrenarYArrancar();
    digitalWrite(DIR1, 1);
    digitalWrite(DIR2, 1);
    digitalWrite(ENABLE, 0);
    GenPasos.enable();
    Serial.println("arrancando");
}

void GirarDerecha()
{
    girando = 1;
    FrenarYArrancar();
    digitalWrite(DIR1, 1);
    digitalWrite(DIR2, 0);
    digitalWrite(ENABLE, 0);
    GenPasos.enable();
    Serial.println("derecha");
}

void GirarIzquierda()
{
    girando = 1;
    FrenarYArrancar();
    digitalWrite(DIR1, 0);
    digitalWrite(DIR2, 1);
    digitalWrite(ENABLE, 0);
    GenPasos.enable();
    Serial.println("izquierda");
}

void InitMotores()
{
    pinMode(DIR1, OUTPUT);
    pinMode(DIR2, OUTPUT);
    pinMode(STEP, OUTPUT);
    pinMode(ENABLE, OUTPUT);
    digitalWrite(ENABLE, 1); // motor parado
    runner.init();
    runner.addTask(GenPasos);
}
