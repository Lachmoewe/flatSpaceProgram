#!/usr/bin/python

import sys, pygame, math, thread
class SpaceObject():
        def __init__(self, imageFile="data/vessel/basic/vessel.png"):
                self.imageFile=imageFile
                self.image=pygame.image.load(imageFile)
                self.rect=self.image.get_rect()
                self.speed=[0.0, 0.0]
                self.position=[0.0, 0.0]
                self.acceleration=[0.0, 0.0]
                self.gravityVec=[0.0,0.0]
                self.mass=1
                self.draw=True
                self.gravInfluence=[]
        
        def addGravityInfluence(self, thing):
                self.gravInfluence.append(thing)

        def calcGravityVec(self):
                for thing in self.gravInfluence:
                        forceVector = [0.0, 0.0]
                        direction = [0.0, 0.0]
                        direction[0] = thing.position[0] - self.position[0]
                        direction[1] = thing.position[1] - self.position[1]
                        distance = math.pow(direction[0],2) + math.pow(direction[1],2)
                        force = thing.mass/distance
                        distance = math.sqrt(distance)
                        forceVector[0] = (direction[0]/distance)*force
                        forceVector[1] = (direction[1]/distance)*force
                        self.gravityVec[0]=self.gravityVec[0]+forceVector[0]
                        self.gravityVec[1]=self.gravityVec[1]+forceVector[1]
        
        def calcAcceleration(self):
                self.acceleration[0] = self.gravityVec[0]
                self.acceleration[1] = self.gravityVec[1]
                self.gravityVec=[0.0, 0.0]
        
        def calcSpeed(self):
                self.speed[0] = self.speed[0] + self.acceleration[0]
                self.speed[1] = self.speed[1] + self.acceleration[1]
        
        def calcPosition(self):
                self.position[0] = self.position[0] + self.speed[0]
                self.position[1] = self.position[1] + self.speed[1]
                self.rect.center=(int(self.position[0]),int(self.position[1]))
                
        def draw(self, screen):
                self.calcPosition()
                screen.blit(self.image, self.rect)

        def tick(self):
                self.calcGravityVec()
                self.calcAcceleration()
                self.calcSpeed()

class Vessel(SpaceObject):
        def __init__(self, imageFile):
                SpaceObject.__init__(self, imageFile)
                self.engineVec=[0.0, 0.0]
                self.enginePower=50

        def calcAcceleration(self):
                SpaceObject.calcAcceleration(self)
                self.acceleration[0] = self.acceleration[0] + self.engineVec[0] * self.enginePower/1000.0
                self.acceleration[1] = self.acceleration[1] + self.engineVec[1] * self.enginePower/1000.0
        

        def setThrust(self,direction):
                if direction == 'u':
                        self.engineVec[1]=self.engineVec[1]+1
                if direction == 'd':
                        self.engineVec[1]=self.engineVec[1]-1
                if direction == 'l':
                        self.engineVec[0]=self.engineVec[0]+1
                if direction == 'r':
                        self.engineVec[0]=self.engineVec[0]-1

        def unsetThrust(self,direction):
                if direction == 'u':
                        self.engineVec[1]=self.engineVec[1]-1
                if direction == 'd':
                        self.engineVec[1]=self.engineVec[1]+1
                if direction == 'l':
                        self.engineVec[0]=self.engineVec[0]-1
                if direction == 'r':
                        self.engineVec[0]=self.engineVec[0]+1

                

class Planet(SpaceObject):
        def __init__(self, imageFile='data/ball.gif'):
                SpaceObject.__init__(self, imageFile)



pygame.init()
size = width, height = 1600, 1000
black = 0, 0, 0

screen = pygame.display.set_mode(size)
pygame.display.set_caption('flatSpaceProgram')

spaceShip=Vessel("data/vessel/basic/vessel.png")

spaceShip.speed=[0.0, 0.2]
spaceShip.position=[160.0, 120.0]
spaceShip.engineVec=[0.0, 0.0]
spaceShip.enginePower=50

moon=Planet("data/planet/moon.png")
moon.position=[200.0, 500.0]
moon.speed=[0.325,0.25]
moon.mass=5

blue=Planet("data/planet/blue.png")
blue.mass=30
blue.position=[100.0,400.0]
blue.speed=[0.0, 0.4]

sun=Planet("data/star/helios1.png")
sun.mass=100
sun.position=[800.0,500.0]
clock=pygame.time.Clock()

spaceShip.addGravityInfluence(blue)
spaceShip.addGravityInfluence(moon)
spaceShip.addGravityInfluence(sun)
moon.addGravityInfluence(blue)
moon.addGravityInfluence(sun)
blue.addGravityInfluence(sun)

myfont=pygame.font.SysFont("Monospace", 20)

def getDistance(vec1, vec2):
        distanceVec=[0.0, 0.0]
        distanceVec[0]=vec2[0]-vec1[0]
        distanceVec[1]=vec2[1]-vec1[1]
        distance=math.sqrt(math.pow(distanceVec[0],2)+math.pow(distanceVec[1],2))
        return distance

def HUD(text,position,screen): 
        lineList=text.split('\n')
        offset=0
        for line in lineList:
                HUD=myfont.render(line,1,(255,0,0))
                screen.blit(HUD,(position[0],position[1]+offset))
                offset=offset+15
while 1:
        for event in pygame.event.get():
                if event.type == pygame.QUIT: sys.exit()
                if event.type == pygame.KEYUP:
                        print 'Key ' + str(event.key) + ' down'
                        if event.key==119:
                                spaceShip.setThrust('u')
                        if event.key==115:
                                spaceShip.setThrust('d')
                        if event.key==97:
                                spaceShip.setThrust('l')
                        if event.key==100:
                                spaceShip.setThrust('r')
                if event.type == pygame.KEYDOWN:
                        print 'Key ' + str(event.key) + ' up'
                        if event.key==119:
                                spaceShip.unsetThrust('u')
                        if event.key==115:
                                spaceShip.unsetThrust('d')
                        if event.key==97:
                                spaceShip.unsetThrust('l')
                        if event.key==100:
                                spaceShip.unsetThrust('r')
        
#spaceShip.tick()
        thread.start_new(spaceShip.tick, ())       
        thread.start_new(moon.tick, ())       
        thread.start_new(blue.tick, ())       
        thread.start_new(sun.tick, ())       
       #moon.tick()
       #blue.tick()
       #sun.tick()

        screen.fill(black)
        spaceShip.calcPosition()
        sun.calcPosition()
        moon.calcPosition()
        blue.calcPosition()
#sun.draw(screen)
       #blue.draw(screen)
       #moon.draw(screen)
       #spaceShip.draw(screen)
        screen.blit(sun.image, sun.rect)
        screen.blit(blue.image, blue.rect)
        screen.blit(moon.image, moon.rect)
        screen.blit(spaceShip.image, spaceShip)

        absSpeed=round(math.sqrt(math.pow(spaceShip.speed[0],2)+math.pow(spaceShip.speed[1],2))*60,2)
        HUDtext='''Speed: '''+str(absSpeed)+'''\n
SpaceShip Pos: (''' + str(round(spaceShip.position[0],1)) + "," + str(round(spaceShip.position[1],1)) + ''')\n
blue<->moon: ''' + str(round(getDistance(blue.position,moon.position),0)) + '''\n
sun<->blue: ''' +str(round(getDistance(sun.position,blue.position),0))
        HUD(HUDtext,[10,10],screen)
        clock.tick(60)
        pygame.display.flip()

