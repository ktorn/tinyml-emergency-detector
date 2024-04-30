/*
* Emergency Gesture Detection - using TinyML
* Requires: XIAO_ESP32C3 and Grove Vision AI (v2)
*
* POC Prototype for ICTP-UNU-TinyML workshop
* 30.04.2024
*
* Authors: gestadieu and ktorn
*/

#include <Seeed_Arduino_SSCMA.h>

SSCMA AI;

const bool DEBUG = false;

const int TICK_DURATION = 1000;
const int TICKS_FOR_EMERGENCY = 4;

const int EMERGENCY_DURATION = 3000;
const int DELAY_BETWEEN_EMERGENCIES = 2000;

const int STATE_NORMAL = 0;
const int STATE_TICKING = 1;
const int STATE_EMERGENCY = 2;

int current_ticks = 0;

unsigned long currentTime = 0;
unsigned long last_tick_time = 0;
unsigned long last_emergency_time = 0;

int state = STATE_NORMAL;

int head_x = 0;
int head_y = 0;

int shoulder_left_x = 0;
int shoulder_left_y = 0;

int shoulder_right_x = 0;
int shoulder_right_y = 0;

int elbow_left_x = 0;
int elbow_left_y = 0;

int elbow_right_x = 0;
int elbow_right_y = 0;

int wrist_left_x = 0;
int wrist_left_y = 0;

int wrist_right_x = 0;
int wrist_right_y = 0;


void setup()
{
    AI.begin();
    Serial.begin(9600);
}

void loop()
{

  currentTime = millis();

  switch(state) {
    case STATE_NORMAL:
      if(DEBUG) {
        Serial.println("State: NORMAL");
      }
    break;

    case STATE_TICKING:
      if(DEBUG) {
        Serial.print("State: TICKING... [");
        Serial.print(current_ticks);
        Serial.println("]");
      }
      if (currentTime > (last_tick_time + TICK_DURATION + 500) ) {
        state = STATE_NORMAL;
        current_ticks = 0;
      }
    break;

    case STATE_EMERGENCY:
      if(DEBUG) {
        Serial.println("State: EMERGENCY");
      }
      if (currentTime > last_emergency_time + EMERGENCY_DURATION) {
        state = STATE_NORMAL;
      }
    break;
  }



    if (!AI.invoke())
    {
      
      if(DEBUG) {
          Serial.println("invoke success");
          Serial.print("perf: prepocess=");
          Serial.print(AI.perf().prepocess);
          Serial.print(", inference=");
          Serial.print(AI.perf().inference);
          Serial.print(", postpocess=");
          Serial.println(AI.perf().postprocess);
      }

        for (int i = 0; i < AI.boxes().size(); i++)
        {

          if(DEBUG) {
              Serial.print("Box[");
              Serial.print(i);
              Serial.print("] target=");
              Serial.print(AI.boxes()[i].target);
              Serial.print(", score=");
              Serial.print(AI.boxes()[i].score);
              Serial.print(", x=");
              Serial.print(AI.boxes()[i].x);
              Serial.print(", y=");
              Serial.print(AI.boxes()[i].y);
              Serial.print(", w=");
              Serial.print(AI.boxes()[i].w);
              Serial.print(", h=");
              Serial.println(AI.boxes()[i].h);
          }
        }

        for (int i = 0; i < AI.classes().size(); i++)
        {
          if(DEBUG) {
              Serial.print("Class[");
              Serial.print(i);
              Serial.print("] target=");
              Serial.print(AI.classes()[i].target);
              Serial.print(", score=");
              Serial.println(AI.classes()[i].score);
          }
        }
        for (int i = 0; i < AI.points().size(); i++)
        {
          if(DEBUG) {
              Serial.print("Point[");
              Serial.print(i);
              Serial.print("]: target=");
              Serial.print(AI.points()[i].target);
              Serial.print(", score=");
              Serial.print(AI.points()[i].score);
              Serial.print(", x=");
              Serial.print(AI.points()[i].x);
              Serial.print(", y=");
              Serial.println(AI.points()[i].y);
          }
        }
        for (int i = 0; i < AI.keypoints().size(); i++)
        {
          if(DEBUG) {
              Serial.print("keypoint[");
              Serial.print(i);
              Serial.print("] target=");
              Serial.print(AI.keypoints()[i].box.target);
              Serial.print(", score=");
              Serial.print(AI.keypoints()[i].box.score);
              Serial.print(", box:[x=");
              Serial.print(AI.keypoints()[i].box.x);
              Serial.print(", y=");
              Serial.print(AI.keypoints()[i].box.y);
              Serial.print(", w=");
              Serial.print(AI.keypoints()[i].box.w);
              Serial.print(", h=");
              Serial.print(AI.keypoints()[i].box.h);
              Serial.print("], total points=");
              Serial.print(AI.keypoints()[i].points.size());
              Serial.print(", points:[");
              for (int j = 0; j < AI.keypoints()[i].points.size(); j++)
              {
                  Serial.print("[");
                  Serial.print(AI.keypoints()[i].points[j].x);
                  Serial.print(",");
                  Serial.print(AI.keypoints()[i].points[j].y);
                  Serial.print("],");
              }
              Serial.println("]");
          }

            head_x = AI.keypoints()[i].points[0].x;
            head_y = AI.keypoints()[i].points[0].y;

            shoulder_left_x = AI.keypoints()[i].points[5].x;
            shoulder_left_y = AI.keypoints()[i].points[5].y;

            shoulder_right_x = AI.keypoints()[i].points[6].x;
            shoulder_right_y = AI.keypoints()[i].points[6].y;

            elbow_left_x = AI.keypoints()[i].points[7].x;
            elbow_left_y = AI.keypoints()[i].points[7].y;

            elbow_right_x = AI.keypoints()[i].points[8].x;
            elbow_right_y = AI.keypoints()[i].points[8].y;

            wrist_left_x = AI.keypoints()[i].points[9].x;
            wrist_left_y = AI.keypoints()[i].points[9].y;

            wrist_right_x = AI.keypoints()[i].points[10].x;
            wrist_right_y = AI.keypoints()[i].points[10].y;

            if(DEBUG) {
                // print key points
                Serial.print("shoulder_left = [");
                Serial.print(shoulder_left_x);
                Serial.print(",");
                Serial.print(shoulder_left_y);
                Serial.print("], ");

                Serial.print("shoulder_right = [");
                Serial.print(shoulder_right_x);
                Serial.print(",");
                Serial.print(shoulder_right_y);
                Serial.print("], ");

                Serial.print("elbow_left = [");
                Serial.print(elbow_left_x);
                Serial.print(",");
                Serial.print(elbow_left_y);
                Serial.print("], ");

                Serial.print("elbow_right = [");
                Serial.print(elbow_right_x);
                Serial.print(",");
                Serial.print(elbow_right_y);
                Serial.print("], ");

                Serial.print("wrist_left = [");
                Serial.print(wrist_left_x);
                Serial.print(",");
                Serial.print(wrist_left_y);
                Serial.print("], ");

                Serial.print("wrist_right = [");
                Serial.print(wrist_right_x);
                Serial.print(",");
                Serial.print(wrist_right_y);
                Serial.println("]");
            }

            if(elbow_left_y < shoulder_left_y && elbow_right_y < shoulder_right_y &&    // elbows above shoulders
               wrist_left_y < elbow_left_y && wrist_right_y < elbow_right_y &&          // wrists above elbows
               head_y - wrist_left_y > 50  && head_y - wrist_right_y > 50  &&           // wrists above head
               wrist_left_x < elbow_left_x && wrist_right_x > elbow_right_x &&          // wrists closer than elbows
               wrist_left_x - wrist_right_x < 100                                       // wrists very close / overlapping
            ) {

              if(DEBUG) {
                Serial.println("Emergency signal detected!");
              }


              switch(state) {
                case STATE_NORMAL:
                  if(currentTime > last_emergency_time + EMERGENCY_DURATION + DELAY_BETWEEN_EMERGENCIES) {
                    state = STATE_TICKING;
                    current_ticks++;
                    last_tick_time = currentTime;
                    Serial.print("Event: TICK [");
                    Serial.print(current_ticks);
                    Serial.println("]");
                  }
                break;

                case STATE_TICKING:
                  if (currentTime > last_tick_time + TICK_DURATION) {
                    current_ticks++;
                    last_tick_time = currentTime;

                    if(current_ticks < TICKS_FOR_EMERGENCY) {
                      Serial.print("Event: TICK [");
                      Serial.print(current_ticks);
                      Serial.println("]");
                    }
                  }

                  if(current_ticks >= TICKS_FOR_EMERGENCY) {
                    state = STATE_EMERGENCY;
                    last_emergency_time = currentTime;
                    current_ticks = 0;
                    Serial.println("Event: EMERGENCY");
                  }
                break;
              }
            }
        }
    }
}
