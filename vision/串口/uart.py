import serial
import struct

def usart_out(a,b,c):
    port = 'com27'  # 串口号
    baudrate = 115200  # 波特率
    ser = serial.Serial(port, baudrate, timeout=2)
    data = struct.pack("<bbhhh",  # 格式为俩个字符俩个短整型(2字节)
                            0x01,  # 帧头1
                            0x01,  # 帧头2
                            int(a),  # up sample by 4   #数据1
                            int(b),  # up sample by 4    #数据2
                            int(c))  # up sample by 4    #数据1
    print(a,b,c)
    ser.write(data)
