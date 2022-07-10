#define	TECHNICAL



#define uS_TO_S_FACTOR 1000000 *60  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  20       /* Time ESP32 will go to sleep (in minutes) */

#define MQTT_USER       "viktor"
#define MQTT_PASSWORD   "viktor"
const char* mqtt_server = "192.168.0.107";


#ifdef TECHNICAL
  #define MQTT_CLIENT_ID  "technical" //MQTT id client
  const char* tempTopic = "sensor/technical/temperature1";
  const char* humTopic = "sensor/technical/humidity";
#endif