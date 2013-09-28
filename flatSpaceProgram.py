#!/usr/bin/python

import sys, pygame, math
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
        
        def addGravityPull(self,vec):
                self.gravityVec[0]=self.gravityVec[0]+vec[0]
                self.gravityVec[1]=self.gravityVec[1]+vec[1]
        
        def calcAcceleration(self):
                self.acceleration[0] = self.gravityVec[0]
                self.acceleration[1] = self.gravityVec[1]
        
        def tick(self):
                self.calcAcceleration()
                self.speed[0] = self.speed[0] + self.acceleration[0]
                self.speed[1] = self.speed[1] + self.acceleration[1]
                self.position[0] = self.position[0] + self.speed[0]
                self.position[1] = self.position[1] + self.speed[1]
                self.rect.center=(int(self.position[0]),int(self.position[1]))
                self.gravityVec=[0.0, 0.0]

class Vessel(SpaceObject):
        def __init__(self, imageFile):
                SpaceObject.__init__(self, imageFile)
                self.engineVec=[0.0, 0.0]
                self.enginePower=50

        def calcAcceleration(self):
                self.acceleration[0] = self.gravityVec[0] + self.engineVec[0] * self.enginePower/1000.0
                self.acceleration[1] = self.gravityVec[1] + self.engineVec[1] * self.enginePower/1000.0
        

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

        def getGravityForce(self, position):
                forceVector = [0.0, 0.0]
                direction = [0.0, 0.0]
                direction[0] = position[0] - self.position[0]
                direction[1] = position[1] - self.position[1]
                distance = math.pow(direction[0],2) + math.pow(direction[1],2)
                force = self.mass/distance
                distance = math.sqrt(distance)
                forceVector[0] = -(direction[0]/distance)*force
                forceVector[1] = -(direction[1]/distance)*force
                return forceVector

def getDistance(vec1, vec2):
        distanceVec=[0.0, 0.0]
        distanceVec[0]=vec2[0]-vec1[0]
        distanceVec[1]=vec2[1]-vec1[1]
        distance=math.sqrt(math.pow(distanceVec[0],2)+math.pow(distanceVec[1],2))
        return distance

pygame.init()
size = width, height = 1000, 800
black = 0, 0, 0

screen = pygame.display.set_mode(size)
pygame.display.set_caption('flatSpaceProgram')

spaceShip=Vessel("data/vessel/basic/vessel.png")

spaceShip.speed=[0.0, 0.2]
spaceShip.position=[160.0, 120.0]
spaceShip.engineVec=[0.0, 0.0]
spaceShip.enginePower=50

moon=Planet("data/planet/moon.png")
moon.position=[500.0, 500.0]
moon.speed=[0.325,-0.325]
moon.mass=5

blue=Planet("data/planet/blue.png")
blue.mass=30
blue.position=[400.0,400.0]

clock=pygame.time.Clock()

myfont=pygame.font.SysFont("Monospace", 20)
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
        
        spaceShip.addGravityPull( blue.getGravityForce( spaceShip.position ) )
        spaceShip.addGravityPull( moon.getGravityForce( spaceShip.position ) )
        moon.addGravityPull( blue.getGravityForce( moon.position ) )
        spaceShip.tick()
        moon.tick()
        blue.tick()

        #print round(getDistance(moon.position, blue.position))

        screen.fill(black)
        screen.blit(blue.image, blue.rect)
        screen.blit(moon.image, moon.rect)
        screen.blit(spaceShip.image, spaceShip.rect)
        
        absSpeed=round(math.sqrt(math.pow(spaceShip.speed[0],2)+math.pow(spaceShip.speed[1],2))*60,2)
        speed=myfont.render("Speed: "+str(absSpeed),1,(255,0,0))
        position=myfont.render("Pos: ("+str(round(spaceShip.position[0],1))+","+str(round(spaceShip.position[1],1))+")",1,(255,0,0))
        screen.blit(speed,(10,10))
        screen.blit(position,(10,40))
        
        clock.tick(60)
        pygame.display.flip()

