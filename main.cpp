#include "mbed.h"
#include "platform/mbed_thread.h"

#include "xbee.hpp"
#include "ssWiSocket.hpp"
#include "motor.hpp"
#include "gp2d12.hpp"

#include "atomic"

#define NUM_SAMPLES_TO_BREAK    5
#define DISTANCE_TO_BREAK       20.0
#define SENSING_PERIOD_MS       100
#define SENSING_UPLOAD_SAMPLES  10 //upload every second
#define SENSOR_PORT_ID          1
#define MOTOR_PERIOD_MS         250
#define MOTOR_RIGHT_PORT_ID     3
#define MOTOR_LEFT_PORT_ID      5

std::atomic<bool> closeContact;

void sensors_thread();
void motor_thread();

int main() {
    DigitalOut aliveLed(LED1);
    Thread motorThread;
    Thread sensorThread;
    Thread ledThread;
    Thread radarThread;

    // hw init
    aliveLed = false;
    motor_init();
    // init ssWi protocol
    XBeeModule xbee(p9, p10, 102, 14);
    xbee.setDstAddress(XBeeBroadcastAddress());
    xbee.init(5, 10);
    
    // thread init
    motorThread.start(motor_thread);
    sensorThread.start(sensors_thread);

    // background loop
    while(1) {
        aliveLed = !aliveLed;
        thread_sleep_for(1000);
    }
}

void sensors_thread() {
    int breakCounter = 0;
    closeContact = false;
    double sum = 0.0;
    double sumCounter = 0;
    ssWiSocket* sensorSocket = ssWiSocket::createSocket(SENSOR_PORT_ID);
    
    while (true) {
        double distance;
        
        // read sensor
        distance = gp2d12_read();
        // process input
        breakCounter = (distance <= DISTANCE_TO_BREAK) ? breakCounter + 1 : 0;
        closeContact = breakCounter > NUM_SAMPLES_TO_BREAK;
        sum += distance;
        if (++sumCounter == SENSING_UPLOAD_SAMPLES) {
            // upload average distance
            sensorSocket->write(int(sum/SENSING_UPLOAD_SAMPLES));
            sumCounter = 0;
            sum = 0;
        }

        // thread loop end
        thread_sleep_for(SENSING_PERIOD_MS);
    }
}

void motor_thread() {
    DigitalOut breakLed(LED2);
    ssWiSocket* leftSocket = ssWiSocket::createSocket(MOTOR_LEFT_PORT_ID);
    ssWiSocket* rightSocket = ssWiSocket::createSocket(MOTOR_RIGHT_PORT_ID);
    
    while (true) {
        double speedLeft = 0.0, speedRight = 0.0; 
        bool fwdLeft = true, fwdRight = true;

        if (closeContact) {
            breakLed = 1;
        } else {
            breakLed = 0;
            // read inputs
            speedLeft = leftSocket->read(); // percentage [-100, 100]
            speedRight = rightSocket->read(); // percentage [-100, 100]
            speedLeft /= 100; //[0.0, 1.0]
            speedRight /= 100; //[0.0, 1.0]
            // process input
            fwdLeft = speedLeft >= 0.0;
            fwdRight = speedRight >= 0.0;
            speedLeft = abs(speedLeft);
            speedRight = abs(speedRight);
        }
        // actuator
        set_left_motor(speedLeft, fwdLeft);
        set_right_motor(speedRight, fwdRight);

        // thread loop end
        thread_sleep_for(MOTOR_PERIOD_MS);
    }
}
