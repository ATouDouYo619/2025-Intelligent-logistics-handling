from maix import image, display, app, time, camera, pinmap, uart
from maix import gpio, pwm

from HouFu import detect_circles                  
from Hsv import Find_Hsv_Color,Find_Hsv_XY      

from Pack import print_hex,Data_Pack,Data_circular_ring,Data_Color_Pack

pinmap.set_pin_function("A17", "PWM5")              # A17为PWM
pinmap.set_pin_function("A16", "GPIOA16")           # A16为GPIOA

pinmap.set_pin_function("A18", "UART1_RX")
pinmap.set_pin_function("A19", "UART1_TX")

device = "/dev/ttyS1"
serial1 = uart.UART(device, 9600)

led = gpio.GPIO("GPIOA16", gpio.Mode.OUT)  # Mode.OUT通常默认为推挽输出
led.value(0)                #初始输出低电平

Pwm = pwm.PWM(5,500,0,True,-1)              # 使用PWM通道5（对应A17）

disp = display.Display()  
cam = camera.Camera(320, 240)

running_mode = None

while not app.need_exit():  

    img = cam.read()
    img = img.lens_corr(strength=1.5)

    # led.value(1)
    # Pwm.duty(20)
    # CX,CY = Find_Hsv_XY(disp, img)
    # Hsv_Data = Data_Color_Pack(CX,CY)

    RxData = serial1.read(1)
    if RxData == b'\x01':
        running_mode = 'YanSe'              # 颜色检测模式
    elif RxData == b'\x02':
        running_mode = 'YuanHuan'           # 圆环检测模式
    elif RxData == b'\x04':
        running_mode = 'DuoMa'              # 叠层检测模式
    elif RxData == b'\x03':
        running_mode = None                 # 没有接收到有效数据，显示原图
    else:
        pass

    if running_mode == 'YanSe':
        led.value(0)
        Pwm.duty(0)
        ID, cx, cy = Find_Hsv_Color(disp, img, roi_ratio=0.15)
        YanSe_Data = Data_circular_ring(ID)
        # print_hex(YanSe_Data)
        serial1.write(YanSe_Data)

    elif running_mode == 'YuanHuan':
        led.value(1)
        Pwm.duty(100)              
        Cx, Cy = detect_circles(disp, img)
        YuanHuan_Data = Data_Pack(Cx, Cy)
#        print_hex(YuanHuan_Data)
        serial1.write(YuanHuan_Data)

    elif running_mode == 'DuoMa':
        led.value(1)
        Pwm.duty(20)
        # CX,CY = Find_Hsv_XY(disp, img)
        # Hsv_Data = Data_Color_Pack(CX,CY)
        # print_hex(Hsv_Data)
        ID, X, Y = Find_Hsv_Color(disp, img, roi_ratio=0.9)
        Hsv_Data = Data_Color_Pack(X,Y)
        serial1.write(Hsv_Data)

    else:
        led.value(0)
        Pwm.duty(0)
        disp.show(img)
