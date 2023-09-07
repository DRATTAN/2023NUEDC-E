# Hello World Example
#
# Welcome to the OpenMV IDE! Click on the green run arrow button below to run the script!

import sensor, image, time
from pyb import Servo
from pid import PID
from pyb import UART
from AutoThreshold import Auto_Threshold

uart = UART(3, 115200)
pan_servo = Servo(1)
tilt_servo = Servo(2)
pan_servo.calibration(800,2300,1500)
tilt_servo.calibration(800,2300,1500)
pan_servo.pulse_width(1500)
tilt_servo.pulse_width(1500)
status = -1

pan_pid = PID(p=0.3, i = 0.05, d = 0.024, imax = 1000 )
tilt_pid = PID(p=0.3, i = 0.05, d = 0.024, imax = 1000 )
#pan_pidmin = PID(p=0.22, i = 0.25, d = 0.001, imax = 5 )

TARGET_POINTS = [(0,0),(0,0),(0,0),(0,0)]
RECT_POINTS = [(0,0),(0,0),(0,0),(0,0)]
thresholds = [92, 100, -128, 127, -128, 127]
thresholdsred = (0, 100, 12, 127, 1, 127)
thresholdsredinblack = (0, 100, 12, 127, 1, 127)
thresholdgreen = [60, 100, -128, 127, -128, 127]
thresholdrect = (20, 100, -128, 127, -128, 127)
thresholdblack = (0, 0, -128, 127, -128, 127)
thresholdwhite = (100, 100, -128, 127, -128, 127)

kernel = [2, 1,  0,\
          1,  -1,  -1,\
          0,  -1,  -2]

kernelb = [2, 0,  5,\
          1,  -1,  -1,\
          0,  -1,  2]

sensor.reset() # Initialize the camera sensor
sensor.set_vflip(True)
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QVGA) # use QQVGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(0)
sensor.set_auto_gain(0)
sensor.set_auto_exposure(0, exposure_us = 50000) # 设置的小一点，运行得快
clock = time.clock() # Tracks FPS.

def mid_point(p1, p2):
    return [    [(p1[0][0]+p2[0][0]) // 2, (p1[0][1] + p2[0][1]) // 2],
                [(p1[1][0]+p2[1][0]) // 2, (p1[1][1] + p2[1][1]) // 2],
                [(p1[2][0]+p2[2][0]) // 2, (p1[2][1] + p2[2][1]) // 2],
                [(p1[3][0]+p2[3][0]) // 2, (p1[3][1] + p2[3][1]) // 2]]

def breakpoint(r1,r2):
    while(1):
        img = sensor.snapshot().rotation_corr(corners = TARGET_POINTS).lens_corr(strength = 0.9, zoom = 1.0)
        img = img.draw_rectangle(r1.rect(), color = [255, 0, 0])
        img = img.draw_rectangle(r2.rect(), color = [255, 0, 0])
        for p in r1.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
        for p in r2.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
        continue

def find_maxrect(rects:[rect()]):
    mra = 0
    mr = 0
    for rec in rects:
        if rec[2] * rec[3] > mra:
            mr = rec
            mra = rec[2] * rec[3]
    return mr

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob = blob
            max_size = blob[2] * blob[3]
    return max_blob

def let_blob_goto_point(threshold:list(), pointt, step:int, errors:int, is_steepjump:int):
    thresholds = threshold
    pan_errors = 5
    tilt_errors = 5
    while 1:
        if(uart.any()):
            s = uart.readchar()
            if(s == 0):
                while 1:
                    if(uart.any()):
                        s = uart.readchar()
                        break
        img = sensor.snapshot().rotation_corr(corners = TARGET_POINTS).lens_corr(strength = 0.9, zoom = 1.0)
        blobs = img.find_blobs([thresholds])
        img = img.draw_circle(pointt[0], pointt[1], 5, color = [0, 255, 0])
        if blobs:
            max_blob = find_max(blobs)
            pan_errors = max_blob.cx() - pointt[0]
            tilt_errors = max_blob.cy() - pointt[1]
            img.draw_rectangle(max_blob.rect(), color = [255, 0, 0]) # rect
            uart.write("[%d],[%d],[%d],[%d],[%d], \0"%(pan_errors, tilt_errors, step, errors, is_steepjump))
        if pan_errors >= -1*errors and pan_errors <= errors and tilt_errors >= -1*errors and tilt_errors <= errors:
            break
def let_blob_goto_pointlist(threshold:list(), pointlist, step:int, errors:int, is_steepjump:int):
    for i in pointlist:
        let_blob_goto_point(threshold, i, step, errors, is_steepjump)
    return

def Replenish_Point(p1, p2):
    mid = [(p1[0] + p2[0]) // 2, (p1[1] + p2[1]) // 2]
    midl = [(p1[0] + mid[0]) // 2, (p1[1] + mid[1]) // 2]
    midr = [(p2[0] + mid[0]) // 2, (p2[1] + mid[1]) // 2]
    return [midl,mid,midr]


while(True):
    clock.tick()                    # Update the FPS clock.
    img = sensor.snapshot().histeq(adaptive = True, clip_limit = 3).morph(1, kernel).gaussian(1).lens_corr(strength = 1.0, zoom = 1.2)
    rects = img.find_rects(threshold = 1000)
    if(rects):
        max_rec = find_maxrect(rects)
        maxarea = max_rec.rect()[2] * max_rec.rect()[3]
        img.draw_rectangle(max_rec.rect(), color = [255, 0, 0])
        for p in max_rec.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
        if(maxarea > 20000 ):
            TARGET_POINTS = [
            (max_rec.corners()[0][0] , max_rec.corners()[0][1] ),
            (max_rec.corners()[1][0] , max_rec.corners()[1][1] ),
            (max_rec.corners()[2][0] , max_rec.corners()[2][1] ),
            (max_rec.corners()[3][0] , max_rec.corners()[3][1] )]
            break
sensor.skip_frames(10) # Let new settings take affect.
while(1):
    img = sensor.snapshot().rotation_corr(corners = TARGET_POINTS).lens_corr(strength = 0.7, zoom = 1.0).histeq(adaptive = True, clip_limit = 3).morph(1, kernel).gaussian(1)
    rects = img.find_rects(threshold = 1000)
    if(rects):
        max_rec = find_maxrect(rects)
        maxarea = max_rec.rect()[2] * max_rec.rect()[3]
        img.draw_rectangle(max_rec.rect(), color = [255, 0, 0])
        for p in max_rec.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
        if(maxarea > 50000):
            RECT_POINTS = [max_rec.corners()[0], max_rec.corners()[1], max_rec.corners()[2], max_rec.corners()[3]]
            break

sensor.set_auto_exposure(0, exposure_us = 20000)
p0 = RECT_POINTS[0]
p1 = RECT_POINTS[1]
p2 = RECT_POINTS[2]
p3 = RECT_POINTS[3]


center_points = [(RECT_POINTS[0][0] + RECT_POINTS[1][0]) // 2, (RECT_POINTS[0][1] + RECT_POINTS[3][1]) // 2]
sensor.skip_frames(10)
uart.write("[0],[0],[0],[0],[0],ok!\0")

while(1):
    if(uart.any()): status = uart.readchar()
    img = sensor.snapshot().rotation_corr(corners = TARGET_POINTS).lens_corr(strength = 0.9, zoom = 1.0)
    if status == 0:
        let_blob_goto_point(thresholdsred, center_points, 5, 5 ,1)
        status = -1

    elif status == 2:
        pc = [(p0[0] + p1[0]) // 2, (p0[1] + p1[1]) // 2]
        let_blob_goto_point(thresholdsred, pc, 5, 4, 1)
        let_blob_goto_point(thresholdsred, p0, 5, 4, 1)
        let_blob_goto_point(thresholdsred, p3, 5, 4, 1)
        let_blob_goto_point(thresholdsred, p2, 5, 4, 1)
        let_blob_goto_point(thresholdsred, p1, 5, 4, 1)
        let_blob_goto_point(thresholdsred, pc, 5, 4, 1)
        status = -1

    elif status == 3:
        sensor.set_auto_exposure(0, exposure_us = 30000) # 设置的小一点，运行得快
        #let_blob_goto_point(thresholdsred, center_points, 5, 5)
        while 1:
            rectmaxarea = 0
            img = sensor.snapshot().rotation_corr(corners = TARGET_POINTS).lens_corr(strength = 0.9, zoom = 1.0).morph(1, kernelb)
            #img = img.binary([thresholdrect], invert = 1)

            rects = img.find_rects(threshold = 5000)
            if(rects):
                max_rect = find_maxrect(rects)
                #if(rectmax[2]*rectmax[3] > 10000 and rectmax[2]*rectmax[3] < 25000):
                #blobmaxarea = max_blob[2] * max_blob[3]
                break
        if(150000 > max_rect.magnitude() > 50000):
            boffset = 4
            #img = img.draw_rectangle(max_rect.rect(), color = [255, 0, 0])
            #print(max_rect.magnitude())
            #img = img.draw_rectangle([max_rect[0]+boffset, max_rect[1]+boffset, max_rect[2]-2*boffset, max_rect[3]-2*boffset], color = [255, 0, 0])
            midrect = [ [max_rect.corners()[0][0] + boffset,  max_rect.corners()[0][1] - boffset],
                        [max_rect.corners()[1][0] - boffset,  max_rect.corners()[1][1] - boffset],
                        [max_rect.corners()[2][0] - boffset,  max_rect.corners()[2][1] + boffset],
                        [max_rect.corners()[3][0] + boffset,  max_rect.corners()[3][1] + boffset]]
            let_blob_goto_point(thresholdsredinblack, midrect[0], 3, 2, 1)
            let_blob_goto_point(thresholdsredinblack, midrect[1], 3, 2, 1)
            let_blob_goto_point(thresholdsredinblack, midrect[2], 3, 2, 1)
            let_blob_goto_point(thresholdsredinblack, midrect[3], 3, 2, 1)
            let_blob_goto_point(thresholdsredinblack, midrect[0], 3, 2, 1)
        status = -1

    elif status == 4:
        sensor.set_auto_exposure(0, exposure_us = 40000) # 设置的小一点，运行得快
        while 1:
            rectmaxarea = 0
            img = sensor.snapshot().rotation_corr(corners = TARGET_POINTS).lens_corr(strength = 0.9, zoom = 1.0).morph(1, kernelb)
            rects = img.find_rects(threshold = 3000)
            if(rects):
                max_rect = find_maxrect(rects)
                break
        m1 = max_rect
        boffset = 5
        if(150000 > m1.magnitude() > 50000):
            #img = img.draw_rectangle(m1.rect(), color = [255, 0, 0])
            while 1:
                img = sensor.snapshot().rotation_corr(corners = TARGET_POINTS).lens_corr(strength = 0.9, zoom = 1.0).morph(1, kernelb)
                #img = img.binary([thresholdrect], invert = 1)
                rects = img.find_rects(threshold = 5000,roi = m1.rect())
                if(rects):
                    max_rect = find_maxrect(rects)
                    m2 = max_rect
                    if(m2.magnitude()<m1.magnitude()): break
            #breakpoint(m1,m2)
            mid = mid_point(m2.corners(), m1.corners())
            let_blob_goto_point(thresholdsredinblack, mid[0], 5, 5,1)
            midl = Replenish_Point(mid[0], mid[1])
            let_blob_goto_pointlist(thresholdsredinblack, midl, 3, 1, 0)
            let_blob_goto_point(thresholdsredinblack, mid[1], 3, 1, 0)
            midl = Replenish_Point(mid[1], mid[2])
            let_blob_goto_pointlist(thresholdsredinblack, midl, 3, 1, 0)
            let_blob_goto_point(thresholdsredinblack, mid[2], 3,1, 0)
            midl = Replenish_Point(mid[2], mid[3])
            let_blob_goto_pointlist(thresholdsredinblack, midl, 3, 1,0)
            let_blob_goto_point(thresholdsredinblack, mid[3], 3, 1,0)
            midl = Replenish_Point(mid[3], mid[0])
            let_blob_goto_pointlist(thresholdsredinblack, midl, 3, 1,0)
            let_blob_goto_point(thresholdsredinblack, mid[0], 3, 1,0)
        status = -1











