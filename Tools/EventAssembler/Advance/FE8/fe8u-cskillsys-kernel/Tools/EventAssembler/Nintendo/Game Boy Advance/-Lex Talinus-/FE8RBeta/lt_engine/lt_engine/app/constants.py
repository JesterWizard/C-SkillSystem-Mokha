TILEWIDTH, TILEHEIGHT = 16, 16
TILEX, TILEY = 15, 10
WINWIDTH, WINHEIGHT = int(TILEX * TILEWIDTH), int(TILEY * TILEHEIGHT)
PORTRAIT_WIDTH, PORTRAIT_HEIGHT = 128, 112
COLORKEY = 128, 160, 128
FPS = 60
FRAMERATE = 1000//FPS

AUTOTILE_FRAMES = 16

VERSION = "2024.02.18a"

if __name__ == '__main__':
    print(VERSION)

APP_AUTHOR = "rainlash"
APP_NAME = "Lex Talionis"
