/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-04-23 17:01:25
 */
#ifndef OPS9_HPP
#define OPS9_HPP
#include "HardwareSerial.h"
void ops9_task(void *n);
class ops9 {
	friend void ops9_task(void *n);

  public:
	ops9(HardwareSerial *serial) { _serial = serial; };
	~ops9(){};
	void setup(uint8_t TX_pin, uint8_t RX_pin) {
		_serial->begin(115200, SERIAL_8N1, RX_pin, TX_pin);
		xTaskCreate(ops9_task, "read_data", 4096, this, 2, NULL);
	}
	void setup() {
		_serial->begin(115200);
		xTaskCreate(ops9_task, "read_data", 4096, this, 2, NULL);
	}
	//校准
	void calibrate() { _serial->print("ACTR"); }
	//坐标清零
	void re_zero() { _serial->print("ACT0"); }
	//重新设置航向角
	void reset_yaw(float Yaw) {
		_serial->print("ACTJ");
		_serial->write((uint8_t *)&Yaw, 4);
	}
	void reset_x(float X) {
		_serial->print("ACTX");
		_serial->write((uint8_t *)&X, 4);
	}
	void reset_y(float Y) {
		_serial->print("ACTY");
		_serial->write((uint8_t *)&Y, 4);
	}
	void reset_X_Y(float X, float Y) {
		_serial->print("ACTD");
		_serial->write((uint8_t *)&X, 4);
		_serial->write((uint8_t *)&Y, 4);
	}
	void reset_yaw_x_y(float Yaw, float X, float Y) {
		_serial->print("ACTD");
		_serial->write((uint8_t *)&Yaw, 4);
		_serial->write((uint8_t *)&X, 4);
		_serial->write((uint8_t *)&Y, 4);
	}
	float getYaw() { return yaw; }
	float getPitch() { return pitch; }
	float getRoll() { return roll; }
	float getX() { return x; }
	float getY() { return y; }

  private:
	static void ops9_task(void *n) {
		ops9 *obj = (ops9 *)n;
		while (1) {
			obj->read_data();
		}
	};
	void read_data() {
		uint8_t data[26];
		uint8_t front_data[2];

		while (1) {
			if (_serial->available() >= 28) {
				while (1) {
					front_data[0] = _serial->read();
					front_data[1] = _serial->peek();
					if (front_data[0] == 0x0d && front_data[1] == 0x0a) {
						_serial->read();
						break;
					}
					delay(1);
				}
				_serial->read(data, 26);
				if (data[24] == 0x0a && data[25] == 0x0d) {
					int temp = data[0] + (data[1] << 8) + (data[2] << 16) +
					           (data[3] << 24);
					int *temp2 = &temp;
					yaw = *(float *)temp2;
					temp = data[4] + (data[5] << 8) + (data[6] << 16) +
					       (data[7] << 24);
					pitch = *(float *)temp2;
					temp = data[8] + (data[9] << 8) + (data[10] << 16) +
					       (data[11] << 24);
					roll = *(float *)temp2;
					temp = data[12] + (data[13] << 8) + (data[14] << 16) +
					       (data[15] << 24);
					x = *(float *)temp2;
					temp = data[16] + (data[17] << 8) + (data[18] << 16) +
					       (data[19] << 24);
					y = *(float *)temp2;
					temp = data[20] + (data[21] << 8) + (data[22] << 16) +
					       (data[23] << 24);
					yaw_speed = *(float *)temp2;
				}
			}
			delay(1);
		}
	}
	HardwareSerial *_serial;
	//航向角
	float yaw = 0;
	//俯仰角
	float pitch = 0;
	//横滚角
	float roll = 0;
	//X坐标
	float x = 0;
	//Y坐标
	float y = 0;
	//航向角速度
	float yaw_speed = 0;
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