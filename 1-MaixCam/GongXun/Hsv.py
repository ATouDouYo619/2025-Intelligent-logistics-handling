from maix import image, display, app, time, camera
from maix._maix.image import cv2image, image2cv
import cv2
import numpy as np

def Find_Hsv_Color(disp, img, roi_ratio=0.2):

    #红色色域(1)
    lower_red1 = np.array([0, 120, 70])
    upper_red1 = np.array([10, 255, 255])

    #红色色域(2)
    lower_red2 = np.array([156, 120, 70])
    upper_red2 = np.array([180, 255, 255])

    #绿色色域(1)
    lower_green1 = np.array([78, 43, 46])
    upper_green1 = np.array([99, 255, 255])

    #绿色色域(2)
    lower_green2 = np.array([35, 43, 46])
    upper_green2 = np.array([77, 255, 255])   

    #蓝色色域
    lower_blue1 = np.array([100, 150, 0])
    upper_blue1 = np.array([140, 255, 255])

    lower_combined = np.minimum(lower_green2, lower_green1)
    upper_combined = np.maximum(upper_green2, upper_green1)

    img_cv = image2cv(img)
    h, w = img_cv.shape[:2]
    
    roi_size = int(min(h, w) * roi_ratio)
    x_start = w//2 - roi_size//2
    y_start = h//2 - roi_size//2
    roi = img_cv[y_start:y_start+roi_size, x_start:x_start+roi_size]

    blur_roi = cv2.blur(roi, (7,7))
    Hsv_roi = cv2.cvtColor(blur_roi, cv2.COLOR_BGR2HSV)

    mask1 = cv2.inRange(Hsv_roi, lower_red1, upper_red1)
    mask2 = cv2.inRange(Hsv_roi, lower_red2, upper_red2)
    
    mask_red = cv2.bitwise_or(mask1, mask2)
    mask_green = cv2.inRange(Hsv_roi, lower_combined, upper_combined)
    mask_blue = cv2.inRange(Hsv_roi, lower_blue1, upper_blue1)

    detected_color = None
    x, y = None, None

    for mask, color_name in zip([mask_red, mask_green, mask_blue], ['A', 'B', 'C']):
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        
        if contours:
            largest_contour = max(contours, key=cv2.contourArea)
            M = cv2.moments(largest_contour)
            
            if M['m00'] > 0:
                x_roi = int(M['m10'] / M['m00']) 
                y_roi = int(M['m01'] / M['m00'])
                x = x_start + x_roi
                y = y_start + y_roi
                detected_color = color_name
                break

    cv2.rectangle(img_cv, (x_start, y_start), 
                 (x_start+roi_size, y_start+roi_size), 
                 (0, 255, 0), 2)
    
    if x and y:
        cv2.circle(img_cv, (x, y), 5, (255, 255, 255), -1)

    Hsv_maix_img = cv2image(img_cv)
    disp.show(Hsv_maix_img)
    
    return (detected_color, x, y) if detected_color else (None, None, None)


def Find_Hsv_XY(disp, img):

    lower_red1 = np.array([0, 120, 70])
    upper_red1 = np.array([10, 255, 255])
    lower_red2 = np.array([156, 120, 70])
    upper_red2 = np.array([180, 255, 255])

    lower_green1 = np.array([78, 43, 46])
    upper_green1 = np.array([99, 255, 255])

    lower_green2 = np.array([35, 43, 46])
    upper_green2 = np.array([77, 255, 255])   

    lower_blue1 = np.array([100, 150, 0])
    upper_blue1 = np.array([140, 255, 255])

    lower_combined = np.minimum(lower_green2, lower_green1)
    upper_combined = np.maximum(upper_green2, upper_green1)

    img_cv = image2cv(img)
    img_cv_original = img_cv.copy()
    hsv_img = cv2.cvtColor(img_cv, cv2.COLOR_BGR2HSV)

    mask1 = cv2.inRange(hsv_img, lower_red1, upper_red1)
    mask2 = cv2.inRange(hsv_img, lower_red2, upper_red2)
    
    mask_red = cv2.bitwise_or(mask1, mask2)
    mask_green = cv2.inRange(hsv_img, lower_combined, upper_combined)
    mask_blue = cv2.inRange(hsv_img, lower_blue1, upper_blue1)

    kernel_size = 7
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (kernel_size, kernel_size))

# #############################################################################################
    mask_red = cv2.morphologyEx(mask_red, cv2.MORPH_OPEN, kernel, iterations=1)
    mask_red_1 = cv2.morphologyEx(mask_red, cv2.MORPH_CLOSE, kernel, iterations=2)

    mask_green = cv2.morphologyEx(mask_green, cv2.MORPH_OPEN, kernel, iterations=1)
    mask_green_1 = cv2.morphologyEx(mask_green, cv2.MORPH_CLOSE, kernel, iterations=2)

    mask_blue = cv2.morphologyEx(mask_blue, cv2.MORPH_OPEN, kernel, iterations=1)
    mask_blue_1 = cv2.morphologyEx(mask_blue, cv2.MORPH_CLOSE, kernel, iterations=2)

    merged_binary = cv2.bitwise_or(mask_red_1, mask_green_1)
    merged_binary = cv2.bitwise_or(merged_binary, mask_blue_1)

    # 查找轮廓并过滤小区域
    contours, _ = cv2.findContours(
        merged_binary,
        cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE
    )
    min_area = 100
    valid_contours = [cnt for cnt in contours if cv2.contourArea(cnt) > min_area]

    if valid_contours:
        img_with_circles = img_cv_original.copy()
        centers = []
        for cnt in valid_contours:
            (x, y), radius = cv2.minEnclosingCircle(cnt)
            center = (int(x), int(y))
            radius = int(radius)
            cv2.circle(img_with_circles, center, radius, (0, 255, 0), 8)
            cv2.circle(img_with_circles, center, 3, (0, 0, 255), 4)
            centers.append(center)
            # print("Detected Center:", center)
        
        maix_img = cv2image(img_with_circles)
        disp.show(maix_img)
        return centers[0]
    else:
        maix_img = cv2image(img_cv_original)
        disp.show(maix_img)
        # print("No circles detected")
        return (None, None)