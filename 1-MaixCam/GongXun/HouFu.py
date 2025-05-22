from maix import image, display, app, time, camera  
import cv2  
import numpy as np

min_radius = 48  # 设置最小半径 20,60,40,42,45
max_radius = 59  # 设置最大半径 30,180,70,68,60

def detect_circles(disp, img):

        opencv_img = image.image2cv(img)  
          
        gray_img = cv2.cvtColor(opencv_img, cv2.COLOR_RGB2GRAY)  

        # kernel_open = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (2,2))
        # gray_img = cv2.morphologyEx(gray_img, cv2.MORPH_OPEN, kernel_open)
        kernel_close = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (3,2))
        gray_img = cv2.morphologyEx(gray_img, cv2.MORPH_CLOSE, kernel_close)

        
        #cv2.morphologyEx
        circles = cv2.HoughCircles(
            gray_img, 
            cv2.HOUGH_GRADIENT, 
            dp=1,
            minDist=200,
            param1=180,     # 较高的值使边缘检测更加严格
            param2=10,      # 适当降低param2来增强对小圆的敏感度
            minRadius=min_radius,
            maxRadius=max_radius
        )  
          
        if circles is not None:  
            for i in range(circles.shape[1]):  
                # 获取圆心和半径  
                center = (int(circles[0, i, 0]), int(circles[0, i, 1]))  
                radius = int(circles[0, i, 2])  
  
                cv2.circle(gray_img, center, radius, (0, 255, 0), 2)
                #_, binary_img = cv2.threshold(gray_img, 127, 255, cv2.THRESH_BINARY)

                #maix_binary_img = image.cv2image(binary_img)
                maix_img_1 = image.cv2image(gray_img)
                disp.show(maix_img_1)
                
                print("Center:",center)
                return center
        else:
            maix_img = image.cv2image(gray_img)
            disp.show(maix_img)
          
            return None, None
