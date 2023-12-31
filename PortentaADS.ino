
#include <EMGFilters.h>

#include <filters.h>
//#include <FIRFilter.h> 

#include "PortentaADS1299.h"
PortentaADS1299 ADS;


//Notch Filter
EMGFilters myFilter3;
EMGFilters myFilter4;
EMGFilters myFilter5;
EMGFilters myFilter6;
EMGFilters myFilter7;
EMGFilters myFilter8;
SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;
unsigned long long interval = 1000000ul / sampleRate;
NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;

unsigned long thisTime;                
unsigned long thatTimeL;
unsigned long elapsedTime;
int sampleCounter = 0;                 // used to time the tesing loop
boolean testing = true;               // this flag is set in serialEvent on reciept of prompt

const float cutoff_freq   = 150.0;   //Cutoff frequency in Hz was 150
const float sampling_time = 0.004; //Sampling time in seconds.
Filter fhp(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);
Filter fhp2(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);
Filter fhp3(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);
Filter fhp4(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);
Filter fhp5(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);
Filter fhp6(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);


void setup() {
//  delay(5000);
  // don't put anything before the initialization routine for recommended POR  
  myFilter3.init(sampleRate, humFreq, true, true, true);
  myFilter4.init(sampleRate, humFreq, true, true, true);
  myFilter5.init(sampleRate, humFreq, true, true, true);
  myFilter6.init(sampleRate, humFreq, true, true, true);
  myFilter7.init(sampleRate, humFreq, true, true, true);
  myFilter8.init(sampleRate, humFreq, true, true, true);
  ADS.initialize(4,false);
  Serial.begin(115200);
  //Serial.println("ADS1299-Arduino UNO Example 2"); 
  delay(2000);             
//========================================================================================================================================
//  ADS.verbose = true;      // when verbose is true, there will be Serial feedback 
//  ADS.RESET();             // send RESET command to default all registers
//  ADS.SDATAC();            // exit Read Data Continuous mode to communicate with ADS
//  //ADS.WREG(ID,0b00000001);
//  //ADS.RREG(ID);
//  ADS.WREG(CONFIG1,0b11010110);
//  ADS.WREG(CONFIG2,0b11010101);
//  ADS.WREG(CONFIG3,0b11001010);
//  ADS.WREG(CH1SET,0b10100000);
//  ADS.WREG(CH2SET,0b00100000);
//  ADS.WREG(CH3SET,0b00100000);
//  ADS.WREG(CH4SET,0b00100000);
//  ADS.WREG(CH5SET,0b00100000);
//  ADS.WREG(CH6SET,0b00100000);
//  ADS.WREG(CH7SET,0b00100000);
//  ADS.WREG(CH8SET,0b00100000);
//  ADS.WREG(BIAS_SENSP,0b11111111);
//  ADS.WREG(BIAS_SENSN,0b11111111);
//  ADS.WREG(LOFF_SENSP,0x00);
//  ADS.WREG(LOFF_SENSN,0x00);
//  //ADS.WREG(MISC1,0b00100000);
//  ADS.WREG(CONFIG4,0x02);
//  
//  ADS.RREGS(0x00,0x17);     // read all registers starting at ID and ending at CONFIG4
////  ADS.WREG(CONFIG3,0xE0);  // enable internal reference buffer, for fun
////  ADS.RREG(CONFIG3);       // verify write
//  ADS.RDATAC();            // enter Read Data Continuous mode
//  ADS.START();                    // start sampling at the default rate

//------------------------------------------------EXPERIMENTAL----------------------------------------------------------------------
/*
Jumper Settings:
JP17 : Open
JP1  : 1-2
JP2  : 2-3
JP20 : 1-2
JP6  : 1-2
JP7  : 1-2
JP8  : 2-3
JP3  : Open
JP5  : Open
JP24 : 2-3
JP4  : Close
JP18 : 2-3
JP21 : 1:2
JP22 : 2-3
JP23 : 1-2
 */
//L̥

//=================================================================================================================================  

//==========================================================REFERENCE EXPERIMENTAL (DOGSHIT)=================================================
/*
 * Connect the reference electrode to the REF_ELEC pin on the ADS1299EEG-FE board. This pin is located on the J6 connector, pin 4.
Set the jumper JP7 to 2-3 position. This will route the REF_ELEC pin to the SRB1 pin of the ADS1299 device.
Set the jumper JP8 to 1-2 position. This will disconnect the SRB1 pin from the BIAS_ELEC pin, which is connected to the mid-supply voltage by default.
Set the SRB1 bit in the MISC1 register to 1. This will route the SRB1 pin to the negative input of all channels. You can do this by writing 0b00100000 to the MISC1 register.
Optionally, you can also set the BIASREF bit in the CONFIG3 register to 1. This will use the voltage at the BIASREF pin as the bias reference voltage instead of the internal reference. You can do this by writing 0b11011100 to the CONFIG3 register.
The new register settings for using the external reference electrode mode are:

MISC1 = 0b00100000
CONFIG3 = 0b11011100 (optional)
 */
ADS.verbose = true; // Enable verbose mode for serial feedback
ADS.RESET(); // Send RESET command to default all registers
ADS.SDATAC(); // Exit Read Data Continuous mode to communicate with ADS

ADS.WREG(CONFIG1,0b00000101); // Write to CONFIG1 register, set data rate to 256 SPS
ADS.WREG(CONFIG2,0b11010101); // Write to CONFIG2 register
ADS.WREG(CONFIG3,0b11011100); // Write to CONFIG3 register, enable bias measurement and internal reference buffer

// Write to CHxSET registers, enable all channels and connect them to normal electrode input
ADS.WREG(CH1SET,0b00000000); // Enable channel 1
ADS.WREG(CH2SET,0b00000000); // Enable channel 2
ADS.WREG(CH3SET,0b00000000); // Enable channel 3
ADS.WREG(CH4SET,0b00000000); // Enable channel 4
ADS.WREG(CH5SET,0b00000000); // Enable channel 5
ADS.WREG(CH6SET,0b00000000); // Enable channel 6
ADS.WREG(CH7SET,0b00000000); // Enable channel 7
ADS.WREG(CH8SET,0b00000000); // Enable channel 8

// Write to BIAS_SENSP and BIAS_SENSN registers, select all channels for bias drive signal
ADS.WREG(BIAS_SENSP,0b11111111);
ADS.WREG(BIAS_SENSN,0b11111111);

// Write to LOFF_SENSP and LOFF_SENSN registers, enable lead-off detection on all channels
ADS.WREG(LOFF_SENSP,0b11111111);
ADS.WREG(LOFF_SENSN,0b11111111);

ADS.WREG(MISC1,0b00100000); // Write to MISC1 register, route the SRB1 pin to the negative input of all channels[^1^][1]

ADS.WREG(CONFIG4,0x02); // Write to CONFIG4 register

ADS.RREGS(0x00,0x17); // Read all registers starting at ID and ending at CONFIG4

ADS.RDATAC(); // Enter Read Data Continuous mode
ADS.START(); // Start the ADS1299 device
//=================================================================================================================================

  //Serial.println("Press 'x' to begin test");    // ask for prompt
} // end of setup
unsigned long old_time;
unsigned long new_time;
void loop(){
//
////  delay(26);
//   new_time = millis();
//  
  if (testing){
    //Serial.println("entering test loop");
    while(digitalRead(PENA_DRDY)){            // watch the DRDY pin
      
      }
    ADS.updateChannelData();          // update the channelData array
//    int ads_value_1 =("Channel 1",ADS.channelData[0]);
    int ads_value_2 =("Channel 2",ADS.channelData[1]);
    int ads_value_3 =("Channel 3",ADS.channelData[2]);
    int ads_value_4 =("Channel 4",ADS.channelData[3]);
    int ads_value_5 =("Channel 5",ADS.channelData[4]);
    int ads_value_6 =("Channel 6",ADS.channelData[5]);
    int ads_value_7 =("Channel 7",ADS.channelData[6]);
    int ads_value_8 =("Channel 8",ADS.channelData[7]);
    //Serial.print("Ch1, "); 
//    Serial.print("Ch6, "); 
//    Serial.print("Ch7, "); Serial.println("Ch8");
//    float ads_value_1f = fhp.filterIn(ads_value_1)/1000;
//    float ads_value_2f = fhp6.filterIn(ads_value_2/1000);
    
    
    float ads_value_3f = fhp2.filterIn(ads_value_3)/1000;
    float ads_value_4f = fhp.filterIn(ads_value_4)/1000;
    float ads_value_5f = fhp5.filterIn(ads_value_5)/1000;
    float ads_value_6f = fhp3.filterIn(ads_value_6)/1000;
    float ads_value_7f = fhp4.filterIn(ads_value_7)/1000;
    float ads_value_8f = fhp6.filterIn(ads_value_8)/1000;
//    float ads_value_8f2 =myFilter.update( fhp6.filterIn(ads_value_8))/1000;
//    float notched3f = myFilter3.update(ads_value_3f)/1000;
//    float notched4f = myFilter4.update(ads_value_4f)/1000;
//    float notched5f = myFilter5.update(ads_value_5f)/1000;
//    float notched6f = myFilter6.update(ads_value_6f)/1000;
//    float notched7f = myFilter7.update(ads_value_7f)/1000;
//    float notched8f = myFilter8.update(ads_value_8f)/1000;
//    if(new_time-old_time>26){
//-------------------------------------------------------------------------
//      Serial.print(-12500/1000);
//      Serial.print("\t");
      
      
//      Serial.print(ads_value_3f);
//      Serial.print("\t");
//      Serial.print(ads_value_4f);
//      Serial.print("\t");
//      Serial.print(ads_value_5f);
//      Serial.print("\t");
//      Serial.print(ads_value_6f);
//      Serial.print("\t");
//      Serial.print(ads_value_7f);
//      Serial.print("\t");
//      Serial.print(ads_value_8f);
//      Serial.print("\t");
//-----------------------------------------
      Serial.println(ads_value_3f);
//      Serial.print(",");
//      Serial.println(ads_value_4f);
//-----------------------------------------
//      Serial.print(ads_value_3f+8);
//      Serial.print("\t");
//      Serial.println(ads_value_4f+6);
//      Serial.print("\t");
//      Serial.print(ads_value_5f+4);
//      Serial.print("\t");
//      Serial.print(ads_value_6f-4);
//      Serial.print("\t");
//      Serial.print(ads_value_7f-6);
//      Serial.print("\t");
//      Serial.print(ads_value_8f-8);
//      Serial.print("\t");
//      Serial.println(12500/1000);
////      old_time=new_time;

//--------------------------------------------------------------------

//      Serial.print(notched3f);
//      Serial.print("\t");
//      Serial.print(notched4f);
//      Serial.print("\t");
//      Serial.print(notched5f);
//      Serial.print("\t");
//      Serial.print(notched6f);
//      Serial.print("\t");
//      Serial.print(notched7f);
//      Serial.print("\t");
//      Serial.print(notched8f);
//      Serial.print("\t");
//      Serial.println(12500/1000);
////    }
////    Serial.print("\t");
////    Serial.println(ads_value_3);
//    //Serial.println(ads_value_3);
//    //Serial.println(ads_value_1);
////    Serial.print(",");
//////    Serial.print(ads_value_2);
////////    /**Serial.print(",");
////////    Serial.print(ads_value_3);
////////    Serial.print(",");
////////    Serial.print(ads_value_4);**/
//////    Serial.print(",");
//////    Serial.print(ads_value_5);
////    Serial.print(",");
////    Serial.print(ads_value_6);
////    Serial.print(",");
////    Serial.print(ads_value_7);
////    Serial.print(",");
////    Serial.println(ads_value_8);
//
////    ADS.updateChannelData();  
//    
//     
//    //sampleCounter++;                  // increment sample counter for next time
//    
//    //elapsedTime = thisTime - thatTime;
//    //Serial.print("Elapsed Time ");Serial.println(elapsedTime);  // benchmark
//      //Serial.print("Samples ");Serial.println(sampleCounter);   // 
//    //testing = true;                // reset testing flag
//    //sampleCounter = 0;              // reset counter
//    //Serial.println("Press 'x' to begin test");  // ask for prompt
  }// end of testing
//
////  ADS.STOP();                     // stop the sampling
//ADS.RREGS(0x00,0x17);
//  Serial.println();
//  delay(2000);
  
} // end of loop


void serialEvent(){            // send an 'x' on the serial line to trigger ADStest()
  while(Serial.available()){      
    char inChar = (char)Serial.read();
    if (inChar  == 'x'){
      testing = false;
    }
    if(inChar == 'v'){
      testing = true;
    }
  }
}
