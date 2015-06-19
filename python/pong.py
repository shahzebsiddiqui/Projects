""" 
Author: Shahzeb Siddiqui
Date:   04/19/2014
Description: This is an implementation of the classic arcade game Pong
Note: This code works with codeskulptor.org and will not work with tradition Python compiler because it uses library simplegui
      which is a custom made library
"""
import simplegui
import random

# initialize globals - pos and vel encode vertical info for paddles
WIDTH = 600
HEIGHT = 400       
BALL_RADIUS = 20
PAD_WIDTH = 8
PAD_HEIGHT = 80
HALF_PAD_WIDTH = PAD_WIDTH / 2
HALF_PAD_HEIGHT = PAD_HEIGHT / 2
LEFT = False
RIGHT = True


BALL_WIDTH = 4
ball_pos = [WIDTH/2,HEIGHT/2]
ball_vel = [0,0]
paddle1_vel = paddle2_vel = 0
# initialize ball_pos and ball_vel for new bal in middle of table
# if direction is RIGHT, the ball's velocity is upper right, else upper left
def spawn_ball(direction):
    global ball_pos, ball_vel # these are vectors stored as lists
    ball_pos = [WIDTH/2,HEIGHT/2]
    vel_x_dir = random.randrange(2,4)
    vel_y_dir = random.randrange(1,3)
    if direction == 'RIGHT':
        ball_vel = [vel_x_dir,-vel_y_dir]
    else:
        ball_vel = [-vel_x_dir,-vel_y_dir]

# define event handlers
def restart_handler():
    new_game()
    
def new_game():
    global paddle1_pos, paddle2_pos, paddle1_vel, paddle2_vel  # these are numbers
    global score1, score2  # these are ints
    spawn_ball("RIGHT")
    score1 = score2 = 0
    paddle1_pos = paddle2_pos = 160
    

def draw(canvas):
    global score1, score2, paddle1_pos, paddle2_pos, ball_pos, ball_vel
 
        
    # draw mid line and gutters
    canvas.draw_line([WIDTH / 2, 0],[WIDTH / 2, HEIGHT], 1, "White")
    canvas.draw_line([PAD_WIDTH, 0],[PAD_WIDTH, HEIGHT], 1, "White")
    canvas.draw_line([WIDTH - PAD_WIDTH, 0],[WIDTH - PAD_WIDTH, HEIGHT], 1, "White")
        
    # update ball
    ball_pos[0] += ball_vel[0]
    ball_pos[1] += ball_vel[1]

    # collision on top 
    if (ball_pos[1] - BALL_RADIUS) < 0:
      ball_vel[1] *= -1
    # collision on bottom    
    elif (ball_pos[1] + BALL_RADIUS) > HEIGHT:
      ball_vel[1] *= -1
    #left wall colision
    elif (ball_pos[0] - BALL_RADIUS) < PAD_WIDTH:
      #if it hits left paddle
      if (ball_pos[1] > paddle1_pos-2 and ball_pos[1] < paddle1_pos+PAD_HEIGHT+2):
        ball_vel[0] *= -1.10           
      else:
          score2+=1  
          spawn_ball("RIGHT")
    #right collision
    elif (ball_pos[0] + BALL_RADIUS) > WIDTH - PAD_WIDTH:
      #if it hits right paddle
        if (ball_pos[1] > paddle2_pos-2 and ball_pos[1] < paddle2_pos+PAD_HEIGHT+2):
            ball_vel[0] *= -1.10
        else:
            score1+=1
            spawn_ball("LEFT")    

    # draw ball
    canvas.draw_circle(ball_pos,BALL_RADIUS,BALL_WIDTH,'WHITE')
    # update paddle's vertical position, keep paddle on the screen
    if paddle1_pos + paddle1_vel >= 0 and paddle1_pos + paddle1_vel <= HEIGHT-PAD_HEIGHT:
        paddle1_pos += paddle1_vel
    if paddle2_pos + paddle2_vel >= 0 and paddle2_pos + paddle2_vel <= HEIGHT-PAD_HEIGHT:                
        paddle2_pos += paddle2_vel       
        
    # draw paddles
    canvas.draw_polygon([(0,paddle1_pos),(PAD_WIDTH,paddle1_pos),(PAD_WIDTH,paddle1_pos+PAD_HEIGHT),(0,paddle1_pos+PAD_HEIGHT)],6,'RED')
    canvas.draw_polygon([(WIDTH-PAD_WIDTH,paddle2_pos),(WIDTH,paddle2_pos),(WIDTH,paddle2_pos+PAD_HEIGHT),(WIDTH-PAD_WIDTH,paddle2_pos+PAD_HEIGHT)],6,'BLUE')
    # draw scores
    canvas.draw_text(str(score1),[250,50],36,'Green')
    canvas.draw_text(str(score2),[350,50],36,'Green')    
def keydown(key):
    global paddle1_vel, paddle2_vel
    if key == simplegui.KEY_MAP['w']:                      
            paddle1_vel = -6
    elif key == simplegui.KEY_MAP['s']:                
          paddle1_vel = 6
    elif key == simplegui.KEY_MAP['up']:                
            paddle2_vel = -6
    elif key == simplegui.KEY_MAP['down']:                
            paddle2_vel = 6    
            
    
            
        
   
def keyup(key):
    global paddle1_vel, paddle2_vel
    if key == simplegui.KEY_MAP['w']:
        paddle1_vel = 0
    elif key == simplegui.KEY_MAP['s']:
        paddle1_vel = 0
    elif key == simplegui.KEY_MAP['up']:
        paddle2_vel = 0
    elif key == simplegui.KEY_MAP['down']:
        paddle2_vel = 0        

# create frame
frame = simplegui.create_frame("Pong", WIDTH, HEIGHT)
frame.set_draw_handler(draw)
frame.set_keydown_handler(keydown)
frame.set_keyup_handler(keyup)
restart = frame.add_button("Restart",restart_handler)


# start frame
new_game()
frame.start()

