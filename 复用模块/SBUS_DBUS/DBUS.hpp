#ifndef DBUS_HPP
#define DBUS_HPP
#include <Arduino.h>
#include <Arduino.h>

class DBUS {
  public:
	DBUS(uint8_t pin, HardwareSerial *serial) {
		_pin = pin;
		_serial = serial;
	};
	DBUS(uint8_t pin) {
		_pin = pin;
		_serial = &Serial2;
	};
	DBUS() {
		_serial = &Serial2;
		_pin = 6;
	};
	void setup() {
		pinMode(_pin, INPUT);
		_serial->begin(100000, SERIAL_8E2, _pin, 38);
        _serial->setRxBufferSize(18*5);
		memset(raw_data, 0, sizeof(raw_data));
		memset(channel_data, 0, sizeof(channel_data));
		xTaskCreate(loop_task, "loop_task", 4096, this, 2, NULL);
	};
	static void loop_task(void *p) {
        DBUS *obj = (DBUS *)p;
        auto check_S1_data=[](uint8_t s1byte){
            int value=((s1byte >> 4) & 0x000C) >> 2;
            return value<4&&value>0;
        };
        auto check_S2_data=[](uint8_t s2byte){
            int value=(s2byte >> 4) & 0x0003;
            return value<4&&value>0;
        };


        //检查S1和S2的值来对齐字节流
        auto data_check =[obj](){
            if (obj->_serial->available() < 36) {
                delay(1);
                continue;
            }
            while(!check_S1_data(obj->_serial->read())){
                if(check_S2_data(obj->_serial->peek())){
                    for(int i=0;i<13;i++){
                        obj->_serial->read();
                    }
                    break;
                }
                obj->_serial->read();
            }
        };
        
		data_check();
		while (true) {
            if (obj->_serial->available() < 18) {
				delay(1);
				continue;
			}
            obj->_serial->readBytes(obj->raw_data, 18);

			channel_data_check[0] = ((int16_t)obj->raw_data[0] >> 0 |
			                            ((int16_t)obj->raw_data[1] << 8)) &
			                        0x07FF;
			channel_data_check[1] = ((int16_t)obj->raw_data[1] >> 3 |
			                            ((int16_t)obj->raw_data[2] << 5)) &
			                        0x07FF;
			channel_data_check[2] = ((int16_t)obj->raw_data[2] >> 6 |
			                            ((int16_t)obj->raw_data[3] << 2) |
			                            (int16_t)obj->raw_data[4] << 10) &
			                        0x07FF;
			channel_data_check[3] = ((int16_t)obj->raw_data[4] >> 1 |
			                            ((int16_t)obj->raw_data[5] << 7)) &
			                        0x07FF;
            S1=((raw_data[5] >> 4) & 0x000C) >> 2;
            S2=(raw_data[5] >> 4) & 0x0003;


            mouse_x=*((int16_t*)(raw_data+6));
            mouse_y=*((int16_t*)(raw_data+8));
            mouse_z=*((int16_t*)(raw_data+10));
            mouse_left_button=*(bool*)(raw_data+12);
            mouse_right_button=*(bool*)(raw_data+13);
            uint8_t button_byte= raw_data[14];
            button_W=button_byte & 0x01;
            button_S=button_byte & 0x02;
            button_A=button_byte & 0x04;
            button_D=button_byte & 0x08;
            button_Q=button_byte & 0x10;
            button_E=button_byte & 0x20;
            button_Shift=button_byte & 0x40;
            button_Ctrl=button_byte & 0x80;
        }
			
	}
	bool is_first = true;

  protected:
	uint8_t _pin;
	HardwareSerial *_serial;
	uint8_t raw_data[18];
	uint16_t channel_data[4];
    uint8_t S1=0;
    uint8_t S2=0;
    int16_t mouse_x=0;
    int16_t mouse_y=0;
    int16_t mouse_z=0;
    bool mouse_left_button=0;
    bool mouse_right_button=0;
    bool button_W=0;
    bool button_S=0;
    bool button_A=0;
    bool button_D=0;
    bool button_Q=0;
    bool button_E=0;
    bool button_Shift=0;
    bool button_Ctrl=0;
	uint8_t flag;
};

#endif
/*
                                              .=%@#=.                                               
                                            -*@@@@@@@#=.                                            
                                         .+%@@@@@@@@@@@@#=                                          
                                       -#@@@@@@@* =@@@@@@@@*:                                       
                                     =%@@@@@@@@=   -@@@@@@@@@#-                                     
                                  .+@@@@@@@@@@-     .@@@@@@@@@@%=                                   
                                .+@@@@@@@@@@@@-     +@@@@@@@@@@@@@+.                                
                               +@@@@@@@@@@@@@@@    .@@@@@@@@@@@@@@@@+.                              
                             =@@@@@@@@@@@@@@@%-     =%@@%@@@@@@@@@@@@@=                             
                           -%@@@@@@@@@@@@+..     .       -@@@@@@@@@@@@@%-                           
                         .#@@@@@@@@@@@@@#       -@+       +@@@@@@@@@@@@@@#:                         
                        +@@@@@@@@@@@@@@@@+     +@@@+     =@@@@@@@@@@@@@@@@@+                        
                      :%@@@@@@@@@@@@@@@@@+    *@@@@*     =@@@@@@@@@@@@@@@@@@%-                      
                     +@@@@@@@@@@@@@@#+*+-   .#@@@@+       :+*+*@@@@@@@@@@@@@@@*                     
                   :%@@@@@@@@@@@@@@+       :%@@@@-    .-       -@@@@@@@@@@@@@@@%:                   
                  =@@@@@@@@@@@@@@@@-      -@@@@%:    .%@+      =@@@@@@@@@@@@@@@@@=                  
                 *@@@@@@@@@@@@@@@@@@.    =@@@@#.    -@@@@+    =@@@@@@@@@@@@@@@@@@@#                 
               .%@@@@@@@@@@@@@@@@@@+    +@@@@*     =@@@@%:    .#@@@@@@@@@@@@@@@@@@@%.               
              :@@@@@@@@@@@@@@@%:::.    #@@@@+     +@@@@#        .::.*@@@@@@@@@@@@@@@@-              
             -@@@@@@@@@@@@@@@%       .%@@@@=     *@@@@*     +-       *@@@@@@@@@@@@@@@@=             
            =@@@@@@@@@@@@@@@@@#.    -@@@@@-    :%@@@@=    .#@@+     +@@@@@@@@@@@@@@@@@@=            
           =@@@@@@@@@@@@@@@@@@@:    =====.     -+===:     :====     @@@@@@@@@@@@@@@@@@@@+           
          +@@@@@@@@@@@@@@@#%%#-                                     :*%%#%@@@@@@@@@@@@@@@+          
         =@@@@@@@@@@@@@@%.       ...........................              *@@@@@@@@@@@@@@@=         
        =@@@@@@@@@@@@@@@+      .#@@@@@@@@@@@@@@@@@@@@@@@@@@#     .*:      =@@@@@@@@@@@@@@@@-        
       -@@@@@@@@@@@@@@@@@=    .%@@@@@@@@@@@@@@@@@@@@@@@@@@#     :@@@-    =@@@@@@@@@@@@@@@@@@:       
      :@@@@@@@@@@@@@@@@@%.   -@@@@%+=====================:     -@@@@%    :%@@@@@@@@@@@@@@@@@@.      
      %@@@@@@@@@@@@@=-=:    =@@@@#.                           +@@@@#.      -=--%@@@@@@@@@@@@@%      
     #@@@@@@@@@@@@@:       +@@@@*      ............. .       *@@@@*             %@@@@@@@@@@@@@+     
    =@@@@@@@@@@@@@@#.     #@@@@+     +@@@@@@@@@@@@@@@#.    .#@@@@+     +#.     +@@@@@@@@@@@@@@@:    
   .@@@@@@@@@@@@@@@@-   .%@@@@=     *@@@@@@@@@@@@@@@#     :%@@@@-     *@@%:    @@@@@@@@@@@@@@@@%    
   %@@@@@@@@@@@%%%#=   :@@@@@:    .#@@@@+-----------     -@@@@@:     #@@@@=    :#%%%@@@@@@@@@@@@*   
  =@@@@@@@@@@@=       -@@@@%.    :%@@@@-                =@@@@%.    .%@@@@=          :%@@@@@@@@@@@:  
  @@@@@@@@@@@%.      =@@@@#     -@@@@%:    .:::-:      +@@@@#     :@@@@@:    .       +@@@@@@@@@@@#  
 +@@@@@@@@@@@@@.    *@@@@*     =@@@@#.    -@@@@@:     #@@@@+     =@@@@%.    -@#     +@@@@@@@@@@@@@- 
.@@@@@@@@@@@@@#    *@%@%=     +@@@@*     =@@@@#.    .#@@@%=     +@@@@#     =@@@%.   =@@@@@@@@@@@@@% 
+@@@@@@@@*-==-                .          .           . ..       .....      .....     .=+=+@@@@@@@@@-
%@@@@@@@+                                                                                 -@@@@@@@@#
@@@@@@@-       =#%#=     -#%%#-     -#%%*.     +%%%*.    .*%%#=     :#%%#-     =%%%*.      .#@@@@@@@
@@@@@@=.::::::*@@@@@*:::-@@@@@@-:::=@@@@@%::::*@@@@@#::::%@@@@@+:---@@@@@@=---+@@@@@%------:=@@@@@@@
=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@%%##**++===----:::::------===++***##%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@* 
  -#@@@@@@@@@@@@@@@@%#*+=-:.                                        ..::-=+*##%@@@@@@@@@@@@@@@@@#-  
    :=*%@@@@@%#*=-:                                                             .:-=+*#%%%%##+-.    
                                                                                        
        K####      #####     ###    ###  ######.   ##########     K##    ### ###    ##W    ####W    
       #######    #######    ###    ###  ########  ##########     ###    ### ###   ###   W######    
      W###G####  ###W ####   ###    ###  ######### ##########     ###    ###  ##   ###   ###W####   
      ###   ###  ###   ###   ###    ##  ###    ###    ###         ###    ###  ### t##   ###   ###   
     G##    #   ###    ###   ##     ##  ###    ###    ###         ###    ###  ### ###   ##W         
     ###        ###    ###   ##    ###  ###    ###    ###         ##L    ##   ### ##   ###          
     ###        ###    ###  K##    ###  ###    ###    ###         ##     ##    #####   ###          
     ###       ,##     ###  ###    ###  ###   ###,    ##         G##    ###    ####    ###          
    W##        ###     ###  ###    ###  #########     ##         ##########    ####    ###          
    ###        ###     ###  ###    ###  ########     ###         ##########    ###i   K##           
    ###        ###     ###  ###    ##  #######       ###         ###    ###    ####   ###           
    ###        ###     ###  ##     ##  ###           ###         ###    ###   ##W##   ###           
    ###        ###     ##i  ##    ###  ###           ###         ###    ##    ## ##   ###           
    ###        ###    ###  ,##    ###  ###           ###         ##     ##   ### ##   ###           
    ###    ### ###    ###  K##    ###  ###           ##         t##    ###   ##  ###  ###    ###    
    ###   G##i ###   ###   .##   ###.  ##t           ##         ###    ###  ###  ###  W##,   ###    
     ########  W##W#####    ########   ##           ###         ###    ###  ##    ##   ####W###     
     #######    #######     #######   ###           ###         ###    ### ###    ##.  #######      
      #####      #####       #####    ###           ###         ###    ### ##W    ###   #####       
                   ###                                                                              
                   ###                                                                              
                   #####                                                                            
                    ####                                                                            
                      K                                                                             
*/