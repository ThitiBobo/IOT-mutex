
/* 
 * Example of using the ChainableRGB library for controlling a Grove RGB.
 * This code cycles through all the colors in an uniform way. This is accomplished using a HSB color space. 
 * comments add By: http://www.seeedstudio.com/
 * Suiable for Grove - Chainable LED 
 */


#include <ChainableLED.h>
//Defines the num of LEDs used, The undefined 
//will be lost control.
#define NUM_LEDS  4

ChainableLED leds(A0, A1, NUM_LEDS);//defines the pin used on arduino.


TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;

static SemaphoreHandle_t mutex;

struct Color{
    int r,g,b;
};


void setup()
{
    Serial.begin(9600);
    struct Color color = createColor(0, 0, 0);
    for (byte i=0; i<NUM_LEDS; i++)
      setColor(i, color);
    
    mutex = xSemaphoreCreateMutex();
    Serial.println("start");
    delay(3000);

    xTaskCreatePinnedToCore(task1Func, "Task1", 10000, NULL, 8, &Task1, 0);
    xTaskCreatePinnedToCore(task2Func, "Task2", 10000, NULL, 8, &Task2, 0);
    xTaskCreatePinnedToCore(task3Func, "Task3", 10000, NULL, 8, &Task3, 0);
}



void task1Func( void * pvParameters ){
  struct Color color1 = createColor(0, 0, 255);
  for(;;){
    add(color1);
    //delay(2000);
    vTaskDelay( pdMS_TO_TICKS( 100 ));
  }
}


void task2Func( void * pvParameters ){
  struct Color color1 = createColor(255, 255, 0);
  for(;;){
      //delay(3000);
      add(color1);
      vTaskDelay( pdMS_TO_TICKS( 100 ) );
  }
}

void task3Func( void * pvParameters ){
  for(;;){
    remove();
    vTaskDelay( pdMS_TO_TICKS( 4000 ) ); 
  }

}





int _max = NUM_LEDS;
int n = 0;
struct Color list[NUM_LEDS];

void loop()
{
    
}





struct Color createColor(int r, int g, int b){
  Color color;
  color.r = r;
  color.g = g;
  color.b = b;
  return color;
}

void add(struct Color color){

  if (xSemaphoreTake(mutex, portMAX_DELAY)){
    if (n < _max){
      // doit faire un for met la flemme 
            list[3] = list[2];
      list[2] = list[1];
      list[1] = list[0];
      list[0] = color;
    
      if (n < _max)n++;
      
      for (byte i=0; i<_max; i++)
        setColor(i, list[i]);
    
      delay(2000);
    }
    xSemaphoreGive(mutex); 
  }  
}

void remove()
{
  xSemaphoreTake(mutex, portMAX_DELAY);
  if (n > 0){
    // doit faire un for met la flemme 
    list[n-1] = createColor(0, 0, 0);
    setColor(n-1, list[n-1]);
  
    if (n > 0) n--;
    delay(2000);
  }
  xSemaphoreGive(mutex);
}

void setColor(int i, struct Color color)
{
  leds.setColorRGB(i, color.r, color.g, color.b);
}
