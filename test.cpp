#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <png.h>

#include <stdio.h>
#include <stdlib.h>
using namespace std;

GLuint PNGtoTexture(char file[]) {
        SDL_Surface *surface;
        GLenum texture_format;
        GLint  nOfColors;
        GLuint texture;

        if ( (surface = IMG_Load(file)) ) { 

                // Check that the image's width is a power of 2
                if ( (surface->w & (surface->w - 1)) != 0 ) {
                        printf("warning: imagefile's width is not a power of 2\n");
                }

                // Also check if the height is a power of 2
                if ( (surface->h & (surface->h - 1)) != 0 ) {
                        printf("warning: imagefile's height is not a power of 2\n");
                }

                // get the number of channels in the SDL surface
                nOfColors = surface->format->BytesPerPixel;
                if (nOfColors == 4)     // contains an alpha channel
                {
                        if (surface->format->Rmask == 0x000000ff)
                                texture_format = GL_RGBA;
                        //else
                        //texture_format = GL_BGRA;
                } else if (nOfColors == 3)     // no alpha channel
                {
                        if (surface->format->Rmask == 0x000000ff)
                                texture_format = GL_RGB;
                        //else
                        //texture_format = GL_BGR;
                } else {
                        printf("warning: the image is not truecolor..  this will probably break\n");
                        // this error should not go unhandled
                }

                // Have OpenGL generate a texture object handle for us
                glGenTextures( 1, &texture );

                // Bind the texture object
                glBindTexture( GL_TEXTURE_2D, texture );

                // Set the texture's stretching properties
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

                // Edit the texture object's image data using the information SDL_Surface gives us
                glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                                texture_format, GL_UNSIGNED_BYTE, surface->pixels );
        } 
        else {
                printf("SDL could not load %s: %s\n",file, SDL_GetError());
                SDL_Quit();
                //return 1;
        }    

        // Free the SDL_Surface only if it was successfully created
        if ( surface ) { 
                SDL_FreeSurface( surface );
        }
        return texture;
}

static GLboolean should_rotate = GL_TRUE;

static void quit_tutorial( int code ) {
        /*
         * Quit SDL so we can release the fullscreen
         * mode and restore the previous video settings,
         * etc.
         */
        SDL_Quit( );

        /* Exit program. */
        exit( code );
}

static void handle_key_down( SDL_Keycode* keysym ) {

        /* 
         * We're only interested if 'Esc' has
         * been presssed.
         *
         * EXERCISE: 
         * Handle the arrow keys and have that change the
         * viewing position/angle.
         */
        switch( *keysym ) {
                case SDLK_ESCAPE:
                        quit_tutorial( 0 );
                        break;
                case SDLK_SPACE:
                        should_rotate = !should_rotate;
                        break;
                default:
                        break;
        }
}

static void process_events( void ) {
        /* Our SDL event placeholder. */
        SDL_Event event;

        /* Grab all the events off the queue. */
        while( SDL_PollEvent( &event ) ) {

                switch( event.type ) {
                        case SDL_KEYDOWN:
                                /* Handle key presses. */
                                handle_key_down( &event.key.keysym.sym );
                                break;
                        case SDL_QUIT:
                                /* Handle quit requests (like Ctrl-c). */
                                quit_tutorial( 0 );
                                break;
                }
        }
}

static void draw_screen( void ) {
        /* Our angle of rotation. */
        static float angle = 0.0f;

        /*
         * EXERCISE:
         * Replace this awful mess with vertex
         * arrays and a call to glDrawElements.
         *
         * EXERCISE:
         * After completing the above, change
         * it to use compiled vertex arrays.
         *
         * EXERCISE:
         * Verify my windings are correct here ;).
         */
        static GLfloat v0[] = { -1.0f, -1.0f,  1.0f };
        static GLfloat v1[] = {  1.0f, -1.0f,  1.0f };
        static GLfloat v2[] = {  1.0f,  1.0f,  1.0f };
        static GLfloat v3[] = { -1.0f,  1.0f,  1.0f };
        static GLfloat v4[] = { -1.0f, -1.0f, -1.0f };
        static GLfloat v5[] = {  1.0f, -1.0f, -1.0f };
        static GLfloat v6[] = {  1.0f,  1.0f, -1.0f };
        static GLfloat v7[] = { -1.0f,  1.0f, -1.0f };
        static GLubyte red[]    = { 255,   0,   0, 255 };
        static GLubyte green[]  = {   0, 255,   0, 255 };
        static GLubyte blue[]   = {   0,   0, 255, 255 };
        static GLubyte white[]  = { 255, 255, 255, 255 };
        static GLubyte yellow[] = {   0, 255, 255, 255 };
        static GLubyte black[]  = {   0,   0,   0, 255 };
        static GLubyte orange[] = { 255, 255,   0, 255 };
        static GLubyte purple[] = { 255,   0, 255,   0 };

        /* Clear the color and depth buffers. */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* We don't want to modify the projection matrix. */
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity( );

        /* Move down the z-axis. */  // not needed becaus of ortho-projection
        //glTranslatef( 0.0, 0.0, -5.0 );

        /* change size */
        glScalef(0.5, 0.5, 0.5);


        /* Rotate. */
        glRotatef( angle, 0.5, 1.0, 0.0 );

        if( should_rotate ) {

                if( ++angle > 360.0f ) {
                        angle = 0.0f;
                }
        }

        /* Send our triangle data to the pipeline. */
        glBegin( GL_TRIANGLES );

        glColor4ubv( red );
        glVertex3fv( v0 );
        glColor4ubv( green );
        glVertex3fv( v1 );
        glColor4ubv( blue );
        glVertex3fv( v2 );

        glColor4ubv( red );
        glVertex3fv( v0 );
        glColor4ubv( blue );
        glVertex3fv( v2 );
        glColor4ubv( white );
        glVertex3fv( v3 );

        glColor4ubv( green );
        glVertex3fv( v1 );
        glColor4ubv( black );
        glVertex3fv( v5 );
        glColor4ubv( orange );
        glVertex3fv( v6 );

        glColor4ubv( green );
        glVertex3fv( v1 );
        glColor4ubv( orange );
        glVertex3fv( v6 );
        glColor4ubv( blue );
        glVertex3fv( v2 );

        glColor4ubv( black );
        glVertex3fv( v5 );
        glColor4ubv( yellow );
        glVertex3fv( v4 );
        glColor4ubv( purple );
        glVertex3fv( v7 );

        glColor4ubv( black );
        glVertex3fv( v5 );
        glColor4ubv( purple );
        glVertex3fv( v7 );
        glColor4ubv( orange );
        glVertex3fv( v6 );

        glColor4ubv( yellow );
        glVertex3fv( v4 );
        glColor4ubv( red );
        glVertex3fv( v0 );
        glColor4ubv( white );
        glVertex3fv( v3 );

        glColor4ubv( yellow );
        glVertex3fv( v4 );
        glColor4ubv( white );
        glVertex3fv( v3 );
        glColor4ubv( purple );
        glVertex3fv( v7 );

        glColor4ubv( white );
        glVertex3fv( v3 );
        glColor4ubv( blue );
        glVertex3fv( v2 );
        glColor4ubv( orange );
        glVertex3fv( v6 );

        glColor4ubv( white );
        glVertex3fv( v3 );
        glColor4ubv( orange );
        glVertex3fv( v6 );
        glColor4ubv( purple );
        glVertex3fv( v7 );

        glColor4ubv( green );
        glVertex3fv( v1 );
        glColor4ubv( red );
        glVertex3fv( v0 );
        glColor4ubv( yellow );
        glVertex3fv( v4 );

        glColor4ubv( green );
        glVertex3fv( v1 );
        glColor4ubv( yellow );
        glVertex3fv( v4 );
        glColor4ubv( black );
        glVertex3fv( v5 );

        glEnd( );

        /*
         * EXERCISE:
         * Draw text telling the user that 'Spc'
         * pauses the rotation and 'Esc' quits.
         * Do it using vetors and textured quads.
         */

        /*
         * Swap the buffers. This this tells the driver to
         * render the next frame from the contents of the
         * back-buffer, and to set all rendering operations
         * to occur on what was the front-buffer.
         *
         * Double buffering prevents nasty visual tearing
         * from the application drawing on areas of the
         * screen that are being updated at the same time.
         */
}

static void setup_opengl( int width, int height ) {
        float ratio = (float) width / (float) height;

        /* Our shading model--Gouraud (smooth). */
        glShadeModel( GL_SMOOTH );

        /* Culling. */
        glCullFace( GL_BACK );
        glFrontFace( GL_CCW );
        glEnable( GL_CULL_FACE );

        /* Set the clear color. */
        glClearColor( 0, 0, 0, 0 );

        /* Setup our viewport. */
        glViewport( 0, 0, width, height );

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();

        /* Orthographic projection yay */
        glOrtho (-ratio, ratio, -1.0, 1.0, 1.5, 20.0);
        gluLookAt (0.0, 0.0, 5.0, 
                   0.5, 0.5, 0.0, 
                   0.0, 1.0, 0.0);
}
void texPos( GLuint texture, float x, float y, float size) {
        /*float left, right, top, bottom;
        left = x + size;
        right = x - size;
        top = y + size;
        bottom = y - size;*/
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin( GL_QUADS );
        /*
        glTexCoord2d(left, top);    glVertex2d(left, top);
        glTexCoord2d(left, bottom); glVertex2d(left, bottom);
        glTexCoord2d(right,bottom); glVertex2d(right,bottom);
        glTexCoord2d(right,top);    glVertex2d(right,top);*/
        
        glTexCoord2d(0.0,0.0); glVertex2d(0.0,0.0);
        glTexCoord2d(1.0,0.0); glVertex2d(1.0,0.0);
        glTexCoord2d(1.0,1.0); glVertex2d(1.0,1.0);
        glTexCoord2d(0.0,1.0); glVertex2d(0.0,1.0);
        glEnd();
}

int main( int argc, char* argv[] ) {

        /*
         * Set our width/height to 1920/1080 (you would
         * of course let the user decide this in a normal
         * app). We get the bpp we will request from
         * the display. On X11, VidMode can't change
         * resolution, so this is probably being overly
         * safe. Under Win32, ChangeDisplaySettings
         * can change the bpp.
         */
        int width = 1920;
        int height = 1080;
        int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;

        SDL_Window *window;
        SDL_GLContext context;

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        window = SDL_CreateWindow("OpenGL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags/*SDL_WINDOW_OPENGL*/);
        if (!window) {
                fprintf(stderr, "Couldn't create window: %s\n", SDL_GetError());
                quit_tutorial(1);
        }

        context = SDL_GL_CreateContext(window);
        if (!context) {
                fprintf(stderr, "Couldn't create context: %s\n", SDL_GetError());
                quit_tutorial(1);
        }

        setup_opengl( width, height );

        int oldTicks = 0;
        int FPS = 0;
        GLuint blue_tex = PNGtoTexture("data/planet/blue_128x128.png");
        float counter = 0.0;
        while( 1 ) {
                process_events( );
                //draw_screen( );
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                glMatrixMode( GL_MODELVIEW );
                glLoadIdentity( );

                texPos(blue_tex, 0, 0, 1);


                glMatrixMode( GL_PROJECTION );
                glLoadIdentity();

                /*gluLookAt (0.0, 0.0, 5.0, 
                           0.5, 0.5, 0.0, 
                           0.0, 1.0, 0.0);
*/
                SDL_GL_SwapWindow( window );
                FPS = 1000/(SDL_GetTicks() - oldTicks);
                //printf("%d\n",FPS);
                oldTicks = SDL_GetTicks();
                printf("%f\n", counter);
                if (counter >= 1) {counter = 0.0;} else {counter = counter+0.001;}
        }

        /*
         * EXERCISE:
         * Record timings using SDL_GetTicks() and
         * and print out frames per second at program
         * end.
         */

        /* Never reached. */
        return 0;
}
