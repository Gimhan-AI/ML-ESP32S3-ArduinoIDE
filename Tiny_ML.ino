#include "model_data.h"
#include <tflm_esp32.h>
#include <eloquent_tinyml.h>

#define NUMBER_OF_INPUTS 2
#define NUMBER_OF_OUTPUTS 1
#define ARENA_SIZE 20000
#define NO_OF_OPS 10

Eloquent::TF::Sequential<NO_OF_OPS, ARENA_SIZE> tf;

void setup() {
    Serial.begin(115200); 
    delay(1000);
    Serial.println("__TENSORFLOW LITE MODEL__");
    tf.setNumInputs(NUMBER_OF_INPUTS);
    tf.setNumOutputs(NUMBER_OF_OUTPUTS);
    while (!tf.begin(converted_model_tflite).isOk()) {
        Serial.println(tf.exception.toString()); 
    }
    Serial.println("Send input data in the format: value1,value2");
}

void loop() {

    if (Serial.available() > 0) {
      
        String data = Serial.readStringUntil('\n');
        
        int commaIndex = data.indexOf(',');
        String input1String = data.substring(0, commaIndex);
        String input2String = data.substring(commaIndex + 1);
        
        float input1 = input1String.toFloat();
        float input2 = input2String.toFloat();
        float input_data[2] = {input1, input2};
        
        unsigned long startTime = micros();
        
        if (!tf.predict(input_data).isOk()) {
            Serial.println(tf.exception.toString()); 
            return; 
        }

        unsigned long endTime = micros();
        
        Serial.print("Predicted Efficiency: ");
        for (int i = 0; i < NUMBER_OF_OUTPUTS; ++i) {
            Serial.print("Output ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(tf.output(i));
        }
        
        Serial.print("Inference Time: ");
        Serial.print(endTime - startTime);
        Serial.println(" ms");
    }

    delay(1000); 
}
