# Untitled - By: User - 週五 六月 4 2021

import sensor, image, time,pyb, math
enable_lens_corr = False # turn on for straighter lines...
sensor.reset()
sensor.set_pixformat(sensor.RGB565) # grayscale is faster
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()

# All lines also have `x1()`, `y1()`, `x2()`, and `y2()` methods to get their end-points
# and a `line()` method to get all the above as one 4 value tuple for `draw_line()`.

uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)

while(True):
   clock.tick()
   img = sensor.snapshot()
   if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...

   # `merge_distance` controls the merging of nearby lines. At 0 (the default), no
   # merging is done. At 1, any line 1 pixel away from another is merged... and so
   # on as you increase this value. You may wish to merge lines as line segment
   # detection produces a lot of line segment results.

   # `max_theta_diff` controls the maximum amount of rotation difference between
   # any two lines about to be merged. The default setting allows for 15 degrees.


   for l in img.find_line_segments(merge_distance = 10, max_theta_diff = 15):
      img.draw_line(l.line(), color = (255, 0, 0))
      #if (l.y2()>100) and ((l.x1()>65 and l.x1()<95) or (l.x2()>65 and l.x2()<95)):
      if (l.y2()>100) and ((l.x1()>20 and l.x1()<140) or (l.x2()>20 and l.x2()<140)):
         print("a")
         print("x1: %.2f" % l.x1())
         print("y1: %.2f" % l.y1())
         print("x2: %.2f" % l.x2())
         print("y2: %.2f" % l.y2())
         # print(l)
         uart.write(("a").encode())
         uart.write(("x1: %.2f" % l.x1()).encode())
         uart.write(("y1: %.2f" % l.y1()).encode())
         uart.write(("x2: %.2f" % l.x2()).encode())
         uart.write(("y2: %.2f" % l.y2()).encode())
         #uart.write(("a: %f" % l.x1()).encode())
   sensor.skip_frames(time = 2000)

    #time.wait(1)
#sensor.skip_frames(time = 2000)
   #print("FPS %f" % clock.fps())
