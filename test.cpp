/*
 * SDL OpenGL Tutorial.
 * (c) Michael Vance, 2000
 * briareos@lokigames.com
 *
 * Distributed under terms of the LGPL. 
 */

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <png.h>

#include <stdio.h>
#include <stdlib.h>


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

        /*
         * Change to the projection matrix and set
         * our viewing volume.
         */
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        
        /* Orthographic projection yay */
        glOrtho (-ratio, ratio, -1.0, 1.0, 1.5, 20.0);
        gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
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

//      printf("Red size: %d, Green size: %d, Blue size: %d\n",
//                      SDL_GL_GetAttribute(SDL_GL_RED_SIZE),
//                      SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE),
//                      SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE));


//      /*
//       * At this point, we should have a properly setup
//       * double-buffered window for use with OpenGL.
//       */
        setup_opengl( width, height );

//      /*
//       * Now we want to begin our normal app process--
//       * an event loop with a lot of redrawing.
//       */
        int oldTicks = 0;
        int FPS = 0;
        while( 1 ) {
                /* Process incoming events. */
                process_events( );
                /* Draw the screen. */
                draw_screen( );
                SDL_GL_SwapWindow( window );
                FPS = 1000/(SDL_GetTicks() - oldTicks);
                printf("%d\n",FPS);
                oldTicks = SDL_GetTicks();
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
