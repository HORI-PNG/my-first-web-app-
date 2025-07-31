import pygame
import sys

pygame.init()
screen = pygame.display.set_mode((800, 600))
clock = pygame.time.Clock()

# ボールの位置と速度
ball_x, ball_y = 400, 300
ball_dx, ball_dy = 5, 3

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    
    # ボールの移動
    ball_x += ball_dx
    ball_y += ball_dy
    
    # 壁で跳ね返り
    if ball_x <= 0 or ball_x >= 780:
        ball_dx = -ball_dx
    if ball_y <= 0 or ball_y >= 580:
        ball_dy = -ball_dy
    
    # 描画
    screen.fill((0, 0, 0))
    pygame.draw.circle(screen, (255, 255, 255), (int(ball_x), int(ball_y)), 20)
    
    pygame.display.flip()
    clock.tick(60)

pygame.quit()
sys.exit()