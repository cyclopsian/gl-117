/*
    GL-117
    Copyright 2001, 2002 Thomas A. Drexl aka heptargon

    This file is part of GL-117.

    GL-117 is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    GL-117 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GL-117; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* This file includes the main program. */

/* To fully understand the GL-117 code, please read the methods from bottom to top! */

/*
TODO:
- southern seashore landscape (additional missions)
- alpine snow landscape
- tree colors (fall, spring), draw more tree textures
- torpedo, water
- little bigger gauss filter for light mask, 2x2 grid does not look good ???
- clouds to fly through
- particle systems
- pseudo random number generator
- ROAM code Norbert (test)
- occlusion culling correction
*/

#ifndef IS_MAIN_H

#include "main.h"

int mousex, mousey;

int debug = 1;
bool multiplayer = false, isserver = false;
int day = 1;
int weather = WEATHER_SUNNY;
float sungamma = 45.0;

int camera = 0;
float camx = 0, camy = 0, camz = 0, camphi = 0, camgamma = 0, camtheta = 0;
float sunlight = 1.0, sunlight_dest = 1.0;

float blackout = 0, redout = 0;

int lighting = 1;
int mode = 0;

// pre-defined screen resolutions (x, y, bpp, fullscreen)
int resolution [4] [4] =
        { { 800, 600, 32, 1 },
          { 1024, 768, 32, 1 },
          { 640, 480, 32, 1 },
          { 640, 480, 32, 0 } };

float nearclippingplane = 0.25; // do NOT lower this!

bool sunblinding = false;

Dirs *dirs;

Server *server = NULL;
Client *client = NULL;

/*#ifdef HAVE_SDL
SDL_Thread *threadnet = NULL;
#endif*/

int game = GAME_INIT;

int debuglevel = LOG_MOST;
int brightness = 0;

SoundSystem *sound = NULL;

GL *gl;

//const int maxchars = 8;
//CTexture *texchar [maxchars];

float getView ()
{
  if (weather == WEATHER_THUNDERSTORM && view > 40.0)
    return 40.0;
  return view;
}

int clouds = 0;

CTexture *texradar1, *texradar2, *texarrow;//, *texcounter;







CTexture *texsun, *texflare1, *texflare2, *texflare3, *texflare4, *texfont1, *textfont2, *texmoon, *texcross, *texcross2, *texranks, *texmedals;

CTexture *texclouds1, *texclouds2, *texclouds3;

PilotList *pilots;

CExplosion *explosion [maxexplosion];
CBlackSmoke *blacksmoke [maxblacksmoke];
GLLandscape *l = NULL;
Font *font1, *font2;


Uint32 lasttime = 0;


class EditField
{
  public:
  char text [256];
  bool active;
  int maxlen, cursorpos;

  EditField ()
  {
    maxlen = 16;
    memset (text, 0, 256);
  }

  EditField (int maxlen)
  {
    if (maxlen > 254) maxlen = 254;
    this->maxlen = maxlen;
    memset (text, 0, 256);
  }
  void init ()
  {
    memset (text, 0, 256);
    cursorpos = 0;
    active = true;
  }
  void event (unsigned char key)
  {
    if (key == 8)
    {
      if (cursorpos > 0)
      {
        cursorpos --;
        text [cursorpos] = '\0';
      }
    }
    if (cursorpos >= maxlen)
      return;
    if (key == 27)
    {
      active = false;
    }
    else if (key == 13)
    {
      active = false;
      text [cursorpos ++] = '\0';
    }
    else if (key >= ' ')
    {
      if (key >= 'a' && key <= 'z')
        key = key - 'a' + 'A';
      if (key >= font1->n + font1->start) return;
      text [cursorpos ++] = key;
    }
  }
  void draw (float x, float y, float z, int timer)
  {
    if (!active)
      return;
    char buf [256];
    memcpy (buf, text, 256);
    if ((timer / timestep) & 1)
    {
      buf [cursorpos] = 'I';
      buf [cursorpos + 1] = '\0';
    }
    font1->drawText (x, y, z, buf);
  }
};

// intro balls, obsolete
/*class InitKugel
{
  public:
  int explosion;
  CModel *m;
  CVector3 tl;
  InitKugel () {}

  InitKugel (CModel *m)
  {
    tl.x = (int) (0.1 * myrandom(800) - 40);
    tl.y = (int) (0.1 * myrandom(800) - 40);
    tl.z = (int) (0.1 * myrandom(800) - 40);
    this->m = m;
    explosion = 0;
  }

  InitKugel (float x, float y, float z, CModel *m)
  {
    tl.x = x;
    tl.y = y;
    tl.z = z;
    this->m = m;
    explosion = 0;
  }
  
  void draw()
  {
    if (explosion > 200) return;
    if (explosion) explosion ++;
    CVector3 tlnull;
    CRotation rot;
    glPushMatrix();
    m->draw3 (&tlnull, &tl, &rot, 0.8, explosion);
    glPopMatrix();
  }
};*/

char *getKeyString (int key, char *str)
{
  if (key == 8) sprintf (str, "%s", "BKSPC");
  else if (key == 13) sprintf (str, "%s", "ENTER");
  else if (key == 32) sprintf (str, "%s", "SPACE");
  else
  {
    int upkey = toupper (key);
    sprintf (str, "%c", upkey);
  }
  return str;
}










CLoad3DS g_Load3ds;
CModel model_fig;
CModel model_figa;
CModel model_figb;
CModel model_figc;
CModel model_figd;
CModel model_fige;
CModel model_figf;
CModel model_figg;
CModel model_figh;
CModel model_figt;
CModel model_cannon1;
CModel model_cannon2;
CModel model_flare1;
CModel model_chaff1;
CModel model_missile1;
CModel model_missile2;
CModel model_missile3;
CModel model_missile4;
CModel model_missile5;
CModel model_missile6;
CModel model_missile7;
CModel model_missile8;
CModel model_flak1;
CModel model_flarak1;
CModel model_tent1;
CModel model_ship1;
CModel model_ship2;
CModel model_gl117;
CModel model_tank1;
CModel model_container1;
CModel model_truck1;
CModel model_pickup1;
CModel model_pickup2;
CModel model_tank2;
CModel model_tent4;
CModel model_hall1;
CModel model_hall2;
CModel model_oilrig;
CModel model_egg;
CModel model_radar;
CModel model_mine1;
CModel model_aster1;
CModel model_base1;
CModel model_barrier1;

DynamicObj *flare [maxflare];
DynamicObj *chaff [maxchaff];
AIObj *missile [maxmissile];
DynamicObj *laser [maxlaser];
AIObj *fighter [maxfighter];
Star *star [maxstar];
DynamicObj *groundobj [maxgroundobj];
Flash *flash1;

Space *space;

AIObj *fplayer;
CSpaceObj *sphere;
CSphere *objsphere;
//CSpaceObj *csphere;
//CSpherePart *cloudsphere;
HighClouds *highclouds;
HighClouds *highclouds2;

//GLLandscape *l;
CModel *obj, *objlaser, *objmissile;
CVector3 *clip1, *clip2, *tlnull, *tlinf, *tlminf;
CRotation *rotnull, *rotmissile;

EditField pilotedit;

GLenum polygonMode = GL_FILL;

GLfloat light_position0[] = { 0.0, 0.5, -1.0, 0 };

#define max(x,y) ((x)>(y)?(x):(y))

int NX, NY;

int joysticks;

Mission *mission = NULL;
Mission *missionnew = NULL;





bool firststart = false;



CColor colorwhite (255, 255, 255, 255);
CColor colorblue (100, 150, 255, 255);
CColor colorgreen (100, 255, 100, 255);
CColor colororange (255, 150, 100, 255);
CColor colorred (255, 0, 0, 255);
CColor coloryellow (255, 255, 0, 200);
CColor colorgrey (150, 150, 150, 200);


void drawRank (float xp, float yp, float zp, int rank, float zoom)
{
  float x = xp / 10.0, y = yp / 10.0, z = zp;
  float tx1 = 0.5 * (rank % 2);
  float tx2 = tx1 + 0.5;
  float ty1 = 0.75 - 0.25 * (rank / 2);
  float ty2 = ty1 + 0.25;
  zoom /= 10;
  gl->enableTextures (texranks->textureID);
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, 255);
  glTexCoord2f (tx1, ty1);
  glVertex3f (x, y, z);
  glTexCoord2f (tx2, ty1);
  glVertex3f (x + zoom * 2, y, z);
  glTexCoord2f (tx2, ty2);
  glVertex3f (x + zoom * 2, y + zoom, z);
  glTexCoord2f (tx1, ty2);
  glVertex3f (x, y + zoom, z);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  gl->disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void drawMedal (float xp, float yp, float zp, int medal, float zoom, int missionid)
{
  if (medal < 0) return;
  if (missionid < MISSION_CAMPAIGN1 || missionid > MISSION_CAMPAIGN2) return;
  float x = xp / 10.0 - zoom / 20, y = yp / 10.0 - zoom / 20, z = zp;
  float tx1 = 0.5 * (medal % 2);
  float tx2 = tx1 + 0.5;
  float ty1 = 0.5 * (medal / 2);
  float ty2 = ty1 + 0.5;
  zoom /= 10;
  gl->enableTextures (texmedals->textureID);
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, 255);
  glTexCoord2f (tx1, ty1);
  glVertex3f (x, y, z);
  glTexCoord2f (tx2, ty1);
  glVertex3f (x + zoom, y, z);
  glTexCoord2f (tx2, ty2);
  glVertex3f (x + zoom, y + zoom, z);
  glTexCoord2f (tx1, ty2);
  glVertex3f (x, y + zoom, z);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  gl->disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void drawMouseCursor ()
{
  glPushMatrix ();
  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glLoadIdentity ();
  gluOrtho2D (0, width, 0, height);
  glScalef (1, -1, 1);
  glTranslatef (0, -height, 0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  gl->enableTextures (texcross2->textureID);
  int crossradius = width / 35;

  glTranslatef (mousex, mousey, 0);
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, 255);
  glTexCoord2i (0, 0);
  glVertex2d (-crossradius, -crossradius);
  glTexCoord2i (0, 1);
  glVertex2d (-crossradius, crossradius);
  glTexCoord2i (1, 1);
  glVertex2d (crossradius, crossradius);
  glTexCoord2i (1, 0);
  glVertex2d (crossradius, -crossradius);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_TEXTURE_2D);
  gl->disableAlphaBlending ();
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();

  glPopMatrix ();
}

int getMedal (int score)
{
  if (score >= 350) return 1;
  else if (score >= 300) return 0;
  else if (score >= 250) return 3;
  else if (score >= 200) return 2;
  return -1;
}

void gl117_rotateColors (int inittimer_gl117)
{
  int i;
  for (i = 0; i < model_gl117.numObjects; i ++)
  {
    for (int i2 = 0; i2 < model_gl117.object [i]->numVertices; i2 ++)
    {
      model_gl117.object [i]->vertex [i2].color.c [0] = (int) (75.0F * SIN(i2 * 100 + inittimer_gl117 / 2) + 155.0F);
      model_gl117.object [i]->vertex [i2].color.c [1] = model_gl117.object [i]->vertex [i2].color.c [0];
      model_gl117.object [i]->vertex [i2].color.c [2] = 100;
    }
  }
}

void adjustBrightness ()
{
  // adjust brightness setting (blending)
  if (brightness < 0)
  {
    glColor4ub (0, 0, 0, -brightness);
    float xf = 2.0, yf = 1.5, zf = 1.0;
    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin (GL_QUADS);
    glVertex3f (-xf, -yf, -zf);
    glVertex3f (-xf, yf, -zf);
    glVertex3f (xf, yf, -zf);
    glVertex3f (xf, -yf, -zf);
    glEnd ();
    glDisable (GL_BLEND);
//    glEnable (GL_DEPTH_TEST);
  }
  else if (brightness > 0)
  {
    glColor4ub (255, 255, 255, brightness);
    float xf = 2.0, yf = 1.5, zf = 1.0;
    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin (GL_QUADS);
    glVertex3f (-xf, -yf, -zf);
    glVertex3f (-xf, yf, -zf);
    glVertex3f (xf, yf, -zf);
    glVertex3f (xf, -yf, -zf);
    glEnd ();
    glDisable (GL_BLEND);
//    glEnable (GL_DEPTH_TEST);
  }
}



const int maxjoysticks = 10;
#ifndef USE_GLUT
SDL_Joystick *sdljoystick [maxjoysticks];
#endif
int sdldisplay = true;
int sdlreshape = true;
Cockpit *cockpit;
float fps = 30;

int glutwindow;




bool initing = true;
int flash = 0;
int lastshield = 0;

void setPlaneVolume ()
{
  if (game == GAME_PLAY)
  {
    int lev = (int) ((float) 128 * fplayer->thrust / fplayer->maxthrust) - 32;
    sound->setVolume (SOUND_PLANE1, lev);
  }
  else
    sound->setVolume (SOUND_PLANE1, 0);
}

void setLightSource (int gamma)
{
  light_position0 [0] = -cosi [gamma];
  light_position0 [1] = sine [gamma];
  light_position0 [2] = 0;
  glLightfv (GL_LIGHT0, GL_POSITION, light_position0);
}

CModel *getModel (int id)
{
  if (id == FIGHTER_FALCON) return &model_fig;
  else if (id == FIGHTER_SWALLOW) return &model_figa;
  else if (id == FIGHTER_HAWK) return &model_figb;
  else if (id == FIGHTER_HAWK2) return &model_figc;
  else if (id == FIGHTER_BUZZARD) return &model_figd;
  else if (id == FIGHTER_CROW) return &model_fige;
  else if (id == FIGHTER_PHOENIX) return &model_figf;
  else if (id == FIGHTER_REDARROW) return &model_figg;
  else if (id == FIGHTER_BLACKBIRD) return &model_figh;
  else if (id == FIGHTER_TRANSPORT) return &model_figt;
  else if (id == MISSILE_AIR1) return &model_missile1;
  else if (id == MISSILE_AIR2) return &model_missile2;
  else if (id == MISSILE_AIR3) return &model_missile3;
  else if (id == MISSILE_GROUND1) return &model_missile4;
  else if (id == MISSILE_GROUND2) return &model_missile5;
  else if (id == MISSILE_DF1) return &model_missile6;
  else if (id == MISSILE_FF1) return &model_missile7;
  else if (id == MISSILE_FF2) return &model_missile8;
  return &model_fig;
}

void game_levelInit ()
{
  int i, i2;

  initing = true;
  flash = 0;

  // init all objects

  for (i = 0; i < maxfighter; i ++)
  {
    fighter [i]->dinit ();
    fighter [i]->aiinit ();
//    fighter [i]->fighterinit ();
    fighter [i]->explode = 0;
    fighter [i]->tl->x = 0;
    fighter [i]->tl->z = -i * 10;
    fighter [i]->party = 0;
    fighter [i]->zoom = 0.35;
    fighter [i]->deactivate ();
    fighter [i]->recheight = 15;
    fighter [i]->recthrust = 0.2;
    fighter [i]->rectheta = 0;
    fighter [i]->o = &model_fig;
  }

  for (i = 0; i < maxgroundobj; i ++)
  {
    groundobj [i]->dinit ();
    groundobj [i]->zoom = 1;
    groundobj [i]->id = STATIC_PASSIVE;
    groundobj [i]->deactivate ();
  }

//  if (l != NULL) delete l;
  if (!multiplayer || isserver || !isserver) // clients do not need the mission
  {
    if (mission != NULL)
    { delete mission; mission = NULL; }
    mission = missionnew;
    missionnew = NULL;
    mission->difficulty = difficulty;
    mission->start ();
  }

  if (clouds == 0) highclouds->setTexture (NULL);
  else if (clouds == 1) highclouds->setTexture (texclouds1);
  else if (clouds == 2) highclouds->setTexture (texclouds2);
  else if (clouds == 3) highclouds->setTexture (texclouds3);

  if (clouds == 0) highclouds2->setTexture (NULL);
  else if (clouds == 1) highclouds2->setTexture (texclouds1);
  else if (clouds == 2) highclouds2->setTexture (texclouds2);
  else if (clouds == 3) highclouds2->setTexture (texclouds3);

  // place missiles to racks
  for (i = 0; i < maxfighter; i ++)
  {
    if (fighter [i]->id >= FIGHTER1 && fighter [i]->id <= FIGHTER2)
      for (i2 = 0; i2 < 4; i2 ++)
      {
        int type = fighter [i]->missilerack [i2];
        CModel *rm = getModel (MISSILE1 + type);
        fighter [i]->refmodel [i2 * 3] = rm;
        fighter [i]->refmodel [i2 * 3 + 1] = rm;
        fighter [i]->refmodel [i2 * 3 + 2] = rm;
        fighter [i]->reftl [i2 * 3 + 1].z = fighter [i]->reftl [i2 * 3].z - 0.04;
        fighter [i]->reftl [i2 * 3 + 2].z = fighter [i]->reftl [i2 * 3].z + 0.04;
        fighter [i]->reftl [i2 * 3].y = fighter [i]->reftl [i2 * 3 + 1].y - 0.04;
        int tmp = fighter [i]->missilerackn [i2];
        fighter [i]->refscale [i2 * 3] = 0.25;
        fighter [i]->refscale [i2 * 3 + 1] = 0.25;
        fighter [i]->refscale [i2 * 3 + 2] = 0.25;
        if (tmp < 3) fighter [i]->refscale [i2 * 3] = 0;
        if (tmp < 2) fighter [i]->refscale [i2 * 3 + 1] = 0;
      }
  }

  // initialize object's height over the surface
  for (i = 0; i < maxfighter; i ++)
  {
    if (fighter [i]->id >= FLAK1 && fighter [i]->id <= FLAK2)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom;
    }
    else if (fighter [i]->id == SHIP_DESTROYER1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 3;
    }
    else if (fighter [i]->id == SHIP_CRUISER)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 3;
    }
    else if (fighter [i]->id >= TANK1 && fighter [i]->id <= TANK2)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 2;
    }
    else if (fighter [i]->id == STATIC_TENT1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z);
    }
    else if (fighter [i]->id == STATIC_CONTAINER1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 2;
    }
    else if (fighter [i]->id == STATIC_HALL1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 3;
    }
    else if (fighter [i]->id == STATIC_HALL2)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 3;
    }
    else if (fighter [i]->id == STATIC_TENT4)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 3;
    }
    else if (fighter [i]->id == STATIC_OILRIG1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 5;
    }
    else if (fighter [i]->id == STATIC_COMPLEX1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 0.55 * fighter [i]->zoom;
    }
    else if (fighter [i]->id == STATIC_RADAR1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 0.7 * fighter [i]->zoom;
    }
    else if (fighter [i]->id == STATIC_BASE1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 0.5 * fighter [i]->zoom;
    }
    else if (fighter [i]->id == STATIC_BARRIER1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 0.3 * fighter [i]->zoom;
    }
    else if (fighter [i]->id == MISSILE_MINE1)
    {
      fighter [i]->tl->y = l->getHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 5 + myrandom (20);
    }
    else if (fighter [i]->id == ASTEROID)
    {
      ;
    }
    else
    {
      fighter [i]->tl->y = l->getHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 20;
    }
  }

  for (i = 0; i < maxlaser; i ++)
  {
    laser [i]->deactivate ();
    if (day)
    {
      laser [i]->o = &model_cannon1;
      laser [i]->zoom = 0.04;
      laser [i]->drawlight = false;
    }
    else
    {
      laser [i]->o = &model_cannon2;
      laser [i]->zoom = 0.08;
    }
  }

  for (i = 0; i < maxmissile; i ++)
  {
    missile [i]->aiinit ();
    missile [i]->deactivate ();
  }

  for (i = 0; i < maxflare; i ++)
  {
    flare [i]->dinit ();
    flare [i]->deactivate ();
  }

  for (i = 0; i < maxchaff; i ++)
  {
    chaff [i]->dinit ();
    chaff [i]->deactivate ();
  }

  for (i = 0; i < maxstar; i ++)
  {
    star [i]->phi = myrandom (360);
    star [i]->gamma = myrandom (85);
    star [i]->size = 0.6 + 0.15 * myrandom (8);
  }

  CColor skycolor;

  if (day && weather == WEATHER_SUNNY)
  {
    if (sungamma < 35)
    {
      skycolor.setColor ((unsigned short) (127 + 70 - 2 * sungamma), 127, 127);
      objsphere->setColor (&skycolor);
    }
    else
    {
      skycolor.setColor (127, 127, 127 + 5);
      objsphere->setColor (&skycolor);
    }
    if (sungamma < 35)
    {
      gl->fogcolor [0] = (float) (127 + 70 - 2 * sungamma) / 256.0;
      gl->fogcolor [1] = 0.5;
      gl->fogcolor [2] = 0.5;
    }
    else
    {
      gl->fogcolor [0] = 0.5;
      gl->fogcolor [1] = 0.5;
      gl->fogcolor [2] = 0.52;
    }
    skycolor.setColor (50, 200, 255);
    objsphere->setNorthPoleColor (&skycolor, 1.8);
    if (sungamma < 35)
    {
      skycolor.setColor ((unsigned short) (180 + 70 - 2 * sungamma), 180, 180);
      objsphere->setPoleColor (90, (int) (90 - sungamma), &skycolor, 0.3);
    }
    else
    {
      skycolor.setColor (200, 200, 200);
      objsphere->setPoleColor (90, (int) (90 - sungamma), &skycolor, 0.3);
    }
  }
  else if (!day && weather == WEATHER_SUNNY)
  {
    skycolor.setColor (64, 64, 64);
    objsphere->setColor (&skycolor);
    gl->fogcolor [0] = 0.25;
    gl->fogcolor [1] = 0.25;
    gl->fogcolor [2] = 0.25;
    if (l->type != LAND_MOON)
    {
      skycolor.setColor (0, 0, 170);
      objsphere->setNorthPoleColor (&skycolor, 1.8);
      skycolor.setColor (64, 64, 64);
      objsphere->setPoleColor (90, (int) (90 - sungamma), &skycolor, 0.3);
    }
    else
    {
      skycolor.setColor (0, 0, 0);
      objsphere->setNorthPoleColor (&skycolor, 1.8);
    }
  }
  else if (day && weather == WEATHER_THUNDERSTORM)
  {
    skycolor.setColor (102, 102, 102);
    objsphere->setColor (&skycolor);
    gl->fogcolor [0] = 0.4;
    gl->fogcolor [1] = 0.4;
    gl->fogcolor [2] = 0.4;
    skycolor.setColor (102, 102, 102);
    objsphere->setNorthPoleColor (&skycolor, 1.8);
  }
  else if (!day && weather == WEATHER_THUNDERSTORM)
  {
    skycolor.setColor (40, 40, 40);
    objsphere->setColor (&skycolor);
    gl->fogcolor [0] = 0.16;
    gl->fogcolor [1] = 0.16;
    gl->fogcolor [2] = 0.16;
    skycolor.setColor (40, 40, 40);
    objsphere->setNorthPoleColor (&skycolor, 1.8);
  }
  else if (weather == WEATHER_CLOUDY)
  {
    skycolor.setColor (20, 20, 20);
    objsphere->setColor (&skycolor);
    gl->fogcolor [0] = 0.08;
    gl->fogcolor [1] = 0.08;
    gl->fogcolor [2] = 0.08;
  }
  glDeleteLists (objsphere->list1, 1);
  glDeleteLists (objsphere->list2, 1);
  glDeleteLists (objsphere->list3, 1);
  objsphere->list1 = -1;
  objsphere->list2 = -1;
  objsphere->list3 = -1;

//  glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
/*  float light_specular [3] = {0.5, 0.5, 0.5};
  glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );*/
  setLightSource ((int) sungamma);
  glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  glEnable (GL_LIGHT0);                // Turn on a light with defaults set
  float light_ambient [4] = {0.2, 0.2, 0.2, 1.0};
  float light_diffuse [4] = {1.0, 1.0, 1.0, 1.0};
  float light_specular [4] = {1.0, 1.0, 1.0, 1.0};
  glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
  glEnable (GL_LIGHTING);                // Turn on lighting
  glEnable (GL_COLOR_MATERIAL);            // Allow color

  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
//  glDisable (GL_DITHER);

//  mission = new MissionDemo1 (space);

  fplayer->missiletype = fplayer->firstMissile ();
  initing = false;
  lastshield = (int) fplayer->shield;

  fps = 30;

/*#ifdef HAVE_SDL_NET
  if (multiplayer)
  {
    if (threadnet != NULL)
      SDL_KillThread (threadnet);
    threadnet = SDL_CreateThread (net_thread_main, NULL);
  }
#endif*/

}

void game_reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (80.0, (float) width / height, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
/*  if (controls == 1)
    SDL_ShowCursor (1);
  else*/
    SDL_ShowCursor (0);
#endif
}

void menu_reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (80.0, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
  SDL_ShowCursor (0);
#endif
}

void credits_reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (80.0, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
  SDL_ShowCursor (0);
#endif
}

void stats_reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (80.0, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
  SDL_ShowCursor (0);
#endif
}

void playRandomMusic ()
{
  int r = myrandom (6);
  if (r == 0)
    sound->loadMusic (MUSIC_DARK1);
  else if (r == 1)
    sound->loadMusic (MUSIC_STANDBY1);
  else if (r == 2)
    sound->loadMusic (MUSIC_ELECTRO1);
  else if (r == 3)
    sound->loadMusic (MUSIC_STARS1);
  else if (r == 4)
    sound->loadMusic (MUSIC_SOFTTEC1);
  else
    sound->loadMusic (MUSIC_AMBIENT1);
  sound->playMusic ();
}

void switch_menu ()
{
  lasttime = 0;
  setLightSource ((int) sungamma);
  game = GAME_MENU;
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
#ifdef HAVE_SDL
  SDL_WM_GrabInput (SDL_GRAB_OFF);
#endif
}

bool ispromoted;
int statsitemselected = 0;

void switch_stats ()
{
  lasttime = 0;
  setLightSource ((int) sungamma);
  game = GAME_STATS;
  statsitemselected = 0;
  int lastrank = pilots->pilot [pilots->aktpilot]->ranking;
  (void) pilots->pilot [pilots->aktpilot]->getRank ();
  ispromoted = false;
  if (lastrank < pilots->pilot [pilots->aktpilot]->ranking)
    ispromoted = true;
  stats_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
  {
    int missionstate = mission->processtimer (1);
    if (missionstate == 1)
      sound->loadMusic (MUSIC_WINNER1);
    else
      sound->loadMusic (MUSIC_LOSER1);
    sound->playMusic ();
  }
}

void createMission (int missionid)
{
  if (missionid >= MISSION_MULTIPLAYER)
  {
    multiplayer = true;
  }
  else
  {
    multiplayer = false;
    isserver = false;
  }
  if (missionnew != NULL) delete missionnew;
  if (missionid == MISSION_DEMO) missionnew = new MissionDemo1 ();
  else if (missionid == MISSION_TEST1) missionnew = new MissionTest1 ();
  else if (missionid == MISSION_TEST2) missionnew = new MissionTest2 ();
  else if (missionid == MISSION_TRANSPORT) missionnew = new MissionTransport ();
  else if (missionid == MISSION_CONVOY) missionnew = new MissionConvoy ();
  else if (missionid == MISSION_DOGFIGHT2) missionnew = new MissionDogfight2 ();
  else if (missionid == MISSION_AIRBATTLE) missionnew = new MissionAirBattle ();
  else if (missionid == MISSION_SADEFENSE) missionnew = new MissionGround1 ();
  else if (missionid == MISSION_SCOUT) missionnew = new MissionScout ();
  else if (missionid == MISSION_BASE) missionnew = new MissionBase ();
  else if (missionid == MISSION_DEFEND1) missionnew = new MissionDefend1 ();
  else if (missionid == MISSION_DOGFIGHT3) missionnew = new MissionDogfight3 ();
  else if (missionid == MISSION_TANK1) missionnew = new MissionTank1 ();
  else if (missionid == MISSION_SHIP1) missionnew = new MissionShip1 ();
  else if (missionid == MISSION_SHIP2) missionnew = new MissionShip2 ();
  else if (missionid == MISSION_SHIP3) missionnew = new MissionShip3 ();
  else if (missionid == MISSION_CANYON1) missionnew = new MissionCanyon1 ();
  else if (missionid == MISSION_CANYON2) missionnew = new MissionCanyon2 ();
  else if (missionid == MISSION_CANYON3) missionnew = new MissionCanyon3 ();
  else if (missionid == MISSION_TUNNEL1) missionnew = new MissionTunnel1 ();
  else if (missionid == MISSION_MOON1) missionnew = new MissionMoonDefense1 ();
  else if (missionid == MISSION_MOON2) missionnew = new MissionMoonDogfight1 ();
  else if (missionid == MISSION_MOON3) missionnew = new MissionMoonBase1 ();
  else if (missionid == MISSION_TUTORIAL) missionnew = new MissionTutorial1 ();
  else if (missionid == MISSION_DOGFIGHT) missionnew = new MissionDogfight1 ();
  else if (missionid == MISSION_FREEFLIGHT1) missionnew = new MissionFreeFlight1 ();
  else if (missionid == MISSION_DEATHMATCH1) missionnew = new MissionDeathmatch1 ();
  else if (missionid == MISSION_DEATHMATCH2) missionnew = new MissionDeathmatch2 ();
  else if (missionid == MISSION_TEAMBASE1) missionnew = new MissionTeamBase1 ();
  else if (missionid == MISSION_WAVES1) missionnew = new MissionWaves1 ();
  else if (missionid == MISSION_MULTIPLAYER_DOGFIGHT) missionnew = new MissionMultiDogfight1 ();
  if (mission != NULL)
  {
    if (mission->id == missionnew->id)
    {
      missionnew->wantfighter = mission->wantfighter;
      missionnew->wantweapon = mission->wantweapon;
    }
  }
}

void switch_mission (int missionid)
{
  lasttime = 0;
  setLightSource (60);
  game = GAME_MISSION;
  createMission (missionid);
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_fame ()
{
  lasttime = 0;
  setLightSource (60);
  game = GAME_FAME;
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_fighter ()
{
  lasttime = 0;
  setLightSource (60);
  game = GAME_FIGHTER;
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_create ()
{
  lasttime = 0;
  setLightSource (60);
  game = GAME_CREATE;
  isserver = true;
  if (server != NULL) delete server;
  server = new Server ();
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_join ()
{
  lasttime = 0;
  setLightSource (60);
  game = GAME_JOIN;
  isserver = false;
  if (client != NULL) delete client;
  client = new Client ();
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

int creditstimer, finishtimer;

void switch_credits ()
{
  lasttime = 0;
  game = GAME_CREDITS;
  creditstimer = 50 * timestep;
  credits_reshape ();
/*  if (sound->musicplaying)
    sound->haltMusic ();
  sound->playMusic (MUSIC_WINNER1);*/
  sound->stop (SOUND_PLANE1);
}

void switch_finish ()
{
  lasttime = 0;
  game = GAME_FINISH;
  creditstimer = 50 * timestep;
  credits_reshape ();
  if (sound->musicplaying)
    sound->haltMusic ();
  sound->loadMusic (MUSIC_ELECTRO1);
  sound->playMusic ();
  sound->stop (SOUND_PLANE1);
}

void switch_quit ()
{
  lasttime = 0;
  game = GAME_QUIT;
}

void switch_game ()
{
  lasttime = 0;
  setLightSource ((int) sungamma);
  game = GAME_PLAY;
  game_reshape ();
  if (sound->musicplaying)
    sound->haltMusic ();
  sound->playLoop (SOUND_PLANE1);
  setPlaneVolume ();
#ifdef HAVE_SDL
  SDL_WM_GrabInput (SDL_GRAB_ON);
#endif
}

bool startcannon = false;

void event_fireCannon ()
{
  if (!fplayer->active) return;
  if (fplayer->firecannonttl > 0) return;
  if (fplayer->ammo == 0) return;
#ifdef USE_GLUT
  fplayer->fireCannon (laser);
#else
  fplayer->autofire = true;
#endif
}

void event_stopCannon ()
{
  fplayer->autofire = false;
}

void event_fireMissile ()
{
  if (!fplayer->active) return;
  if (fplayer->firemissilettl > 0) return;
  if (fplayer->fireMissile (fplayer->missiletype + MISSILE1, missile))
    sound->play (SOUND_MISSILE1);
}

void event_fireChaff ()
{
  if (!fplayer->active) return;
  if (fplayer->firechaffttl > 0) return;
  if (fplayer->fireChaff (chaff, missile))
    sound->play (SOUND_CHAFF1);
}

void event_fireFlare ()
{
  if (!fplayer->active) return;
  if (fplayer->fireflarettl > 0) return;
  if (fplayer->fireFlare (flare, missile))
    sound->play (SOUND_CHAFF1);
}

void event_selectMissile ()
{
  if (!fplayer->active) return;
  fplayer->missiletype = fplayer->nextMissile (fplayer->missiletype);
}

void event_targetNearest ()
{
  if (!fplayer->active) return;
  fplayer->targetNearestEnemy ((AIObj **) fighter);
  sound->play (SOUND_CLICK1);
}

void event_targetNext ()
{
  if (!fplayer->active) return;
  fplayer->targetNext ((AIObj **) fighter);
  sound->play (SOUND_CLICK1);
}

void event_targetPrevious ()
{
  if (!fplayer->active) return;
  fplayer->targetPrevious ((AIObj **) fighter);
  sound->play (SOUND_CLICK1);
}

void event_targetLocking ()
{
  if (!fplayer->active) return;
  fplayer->targetLockingEnemy ((AIObj **) fighter);
  sound->play (SOUND_CLICK1);
}

void event_thrustUp ()
{
  if (!fplayer->active) return;
  fplayer->thrustUp ();
  fplayer->thrustUp ();
}

void event_thrustDown ()
{
  if (!fplayer->active) return;
  fplayer->thrustDown ();
  fplayer->thrustDown ();
}

void game_key (unsigned char key, int x, int y)
{
  int hikey = toupper (key);
  int lokey = tolower (key);
  if (camera == 50 && game == GAME_PAUSE)
  {
    camera = 0;
    game = GAME_PLAY;
    return;
  }
/*  if (key == 'v')
  {
    view += 10.0;
    if (view > 100) view = 20;
    game_reshape ();
    sound->play (SOUND_CLICK1);
  }
  else if (key == 'q')
  {
    quality ++;
    if (quality > 5) quality = 0;
    if (quality <= 1) space->drawlight = false;
    else space->drawlight = true;
    sound->play (SOUND_CLICK1);
//      printf ("\nQuality = %d", quality); fflush (stdout);
  }*/
  else if (key == 27)
  {
    switch_menu ();
  }
  else if (hikey == key_firemissile || lokey == key_firemissile)
  {
    event_fireMissile ();
  }
  else if (hikey == key_dropflare || lokey == key_dropflare)
  {
    event_fireFlare ();
  }
  else if (hikey == key_dropchaff || lokey == key_dropchaff)
  {
    event_fireChaff ();
  }
  else if (hikey == key_firecannon || lokey == key_firecannon)
  {
    event_fireCannon ();
  }
  else if (hikey == key_selectmissile || lokey == key_selectmissile)
  {
    event_selectMissile ();
  }
  else if (hikey == key_thrustup || lokey == key_thrustup)
  {
    event_thrustUp ();
  }
  else if (hikey == key_thrustdown || lokey == key_thrustdown)
  {
    event_thrustDown ();
  }
/*    else if (key == 'l')
    {
      if (lighting == 0) lighting = 1;
      else lighting = 0;
    }
    else if (key == 'm')
    {
      if (mode == 0) mode = 1;
      else mode = 0;
    }*/
    else if (key >= '1' && key <= '9')
    {
      fplayer->recthrust = fplayer->maxthrust * (1.0 / 18.0 * (key - '0') + 0.5);
      sound->play (SOUND_CLICK1);
    }
/*    else if (key == 'a')
    {
      fplayer->ai = !fplayer->ai;
      if (!fplayer->ai) fplayer->easymodel = 2;
      else fplayer->easymodel = 1;
      if (controls == 100)
        fplayer->easymodel = true;
    }*/
/*    else if (key == 'p')
    {
      if (game == GAME_PLAY) game = GAME_PAUSE;
      else game = GAME_PLAY;
    }*/
/*    else if (key == 'w')
    {
      fplayer->thrustUp ();
      sound->play (SOUND_CLICK1);
      setPlaneVolume ();
    }
    else if (key == 's')
    {
      fplayer->thrustDown ();
      sound->play (SOUND_CLICK1);
      setPlaneVolume ();
    }*/
/*    else if (key == 'n')
    {
      game_levelInit (0);
    }*/
/*    else if (key == '-')
    {
      width -= 50; height -= 50;
      if (width < 50) width = 50;
      if (height < 50) height = 50;
#ifdef USE_GLUT
      glutReshapeWindow (width, height);
#else
      sdlreshape = true;
#endif
    }
    else if (key == '+')
    {
      width += 50; height += 50;
      if (width > 1000) width = 1000;
      if (height > 1000) height = 1000;
#ifdef USE_GLUT
      glutReshapeWindow (width, height);
#else
      sdlreshape = true;
#endif
    }*/
    else if (hikey == key_targetnearest || lokey == key_targetnearest)
    {
      event_targetNearest ();
    }
    else if (hikey == key_targetnext || lokey == key_targetnext)
    {
      event_targetNext ();
    }
    else if (hikey == key_targetprevious || lokey == key_targetprevious)
    {
      event_targetPrevious ();
    }
    else if (hikey == key_targetlocking || lokey == key_targetlocking)
    {
      event_targetLocking ();
    }
/*    else if (key == 'k')
    {
      if (fplayer->target != NULL)
        fplayer->target->shield = 0;
    }*/
/*    else if (key == 'g')
    {
      if (polygonMode==GL_FILL)
        polygonMode=GL_LINE;
      else
        polygonMode=GL_FILL;
      glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
    }*/
#ifdef USE_GLUT
    glutPostRedisplay();
#else
    sdldisplay = true;
#endif
}

void game_keyup (unsigned char key, int x, int y)
{
  int hikey = toupper (key);
  int lokey = tolower (key);
  if (hikey == key_firecannon || lokey == key_firecannon)
  {
    event_stopCannon ();
  }
#ifdef USE_GLUT
    glutPostRedisplay();
#else
    sdldisplay = true;
#endif
}

int aktcam = 0;

void game_keyspecialup (int key, int x, int y)
{
  switch (key) {
  case ' ':
    fplayer->autofire = false;
    break;
  case KEY_UP:
    fplayer->elevatoreffect = 0.0;
    break;
  case KEY_DOWN:
    fplayer->elevatoreffect = 0.0;
    break;
  case KEY_LEFT:
    fplayer->rolleffect = 0;
    break;
  case KEY_RIGHT:
    fplayer->rolleffect = 0;
    break;
#ifdef HAVE_SDL
  case KEY_PGUP:
  case KEY_LALT:
    fplayer->ruddereffect = 0;
    break;
  case KEY_PGDOWN:
  case KEY_LCTRL:
    fplayer->ruddereffect = 0;
    break;
#endif
  }
}

void game_keyspecial (int key, int x, int y)
{
  if (camera == 50 && game == GAME_PAUSE)
  {
    camera = 0;
    game = GAME_PLAY;
  }
  switch (key) {
  case KEY_UP:
    fplayer->elevatoreffect = -1.0;
    break;
  case KEY_DOWN:
    fplayer->elevatoreffect = 1.0;
    break;
  case KEY_LEFT:
    fplayer->rolleffect = 1.0;
    break;
  case KEY_RIGHT:
    fplayer->rolleffect = -1.0;
    break;
  case KEY_PGUP:
#ifndef USE_GLUT
  case KEY_LALT:
#endif
    fplayer->ruddereffect = 1.0;
    break;
  case KEY_PGDOWN:
#ifndef USE_GLUT
  case KEY_LCTRL:
#endif
    fplayer->ruddereffect = -1.0;
    break;
  case KEY_F1:
    camera = 0;
    game_reshape ();
    break;
  case KEY_F2:
    camera = 1;
    game_reshape ();
    break;
  case KEY_F3:
    camera = 2;
    game_reshape ();
    break;
  case KEY_F4:
    camera = 6;
    game_reshape ();
    break;
  case KEY_F5:
    camera = 7;
    game_reshape ();
    break;
  case KEY_F6:
    camera = 9;
    game_reshape ();
    break;
  case KEY_F7:
    camera = 8;
    game_reshape ();
    break;
  case KEY_F8:
    camera = 5;
    game_reshape ();
    break;
/*  case KEY_F9:
    aktcam ++;
    if (aktcam > 7) aktcam = 0;
    camera = 3;
    break;*/
  default:
    return;
  }
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void game_mouse (int button, int state, int x, int y)
{
  if (state == MOUSE_DOWN && fplayer->active)
  {
    if (button == mouse_firecannon)
    {
      fplayer->autofire = !fplayer->autofire;
    }
    else if (button == mouse_firemissile)
    {
      event_fireMissile ();
    }
  }
}

float dtheta = 0, dgamma = 0;

int lastmousex = 0;

int lastmx = 0, lastmy = 0;

void game_mouserelmotion (int xrel, int yrel)
{
  if (controls != CONTROLS_MOUSE_EXP) return;
  float xr = (float) xrel / width, yr = (float) yrel / height;
  
  float roll = (float) -xr * 20;

  if (roll > 1.0F) roll = 1.0F;
  else if (roll < -1.0F) roll = -1.0F;
//  fplayer->rectheta += roll;
  fplayer->rolleffect = roll;
  if (roll < 1E-3)
    fplayer->ruddereffect = (float) xr * 200;
  else 
    fplayer->ruddereffect = 0.0f; 
  if (fplayer->ruddereffect > 1.0) fplayer->ruddereffect = 1.0;
  else if (fplayer->ruddereffect < -1.0) fplayer->ruddereffect = -1.0;

  fplayer->elevatoreffect = (float) yr * fabs (yr) * 20000;
  if (fplayer->elevatoreffect > 1.0f) fplayer->elevatoreffect = 1.0f; 
  else if (fplayer->elevatoreffect < -0.5f) fplayer->elevatoreffect = -0.5f; 
}

void game_mousemotion (int x, int y)
{
  if (controls != CONTROLS_MOUSE && controls != CONTROLS_MOUSE_REVERSE) return;

//  game_mouserelmotion (x, y);
//  return;

//  float f = (float) width / 240.0;
  float mx = width / 2, my = height / 2;
  float dx = x - mx, dy = my - y;
  if (controls == CONTROLS_MOUSE_REVERSE)
    dy *= -1;
/*  int t = (int) fplayer->theta;
  if (t < 0) t += 360;
  float rx = dx * cosi [t] - dy * sine [t];
  float ry = dx * sine [t] + dy * cosi [t];
  rx += mx;
  ry = my - ry;*/
//  fplayer->rectheta = (float) (width / 2 - rx) / f;
//  fplayer->recgamma = 135.0 + (float) (height - ry) / height * 90.0;

// mouse interface code added by Lourens Veen
  float nx = dx / width; // normalised x-coordinate, -1 at lefthand 
                         // side of the screen, 1 at righthand side 

  if (fabs(nx) < (1.0f/3.0f)) 
  if (nx < 0.0f) 
    fplayer->ruddereffect = 729.0f*nx*(nx+1.0f/3.0f)*(nx+1.0f/3.0f)/4.0f; 
  else 
    fplayer->ruddereffect = 729.0f*nx*(nx-1.0f/3.0f)*(nx-1.0f/3.0f)/4.0f; 
  else 
    fplayer->ruddereffect = 0.0f; 

  const float roll_deadarea = (1.0f/30.0f); 

  if (fabs(nx) > roll_deadarea) 
  { 
    if (nx > 0.0f) nx -= roll_deadarea; 
    else nx += roll_deadarea; 

    if (nx > 0) 
      fplayer->rolleffect = -(exp(log(nx) * 1.3f)) * 3.0f; 
    else 
      fplayer->rolleffect = (exp(log(-nx) * 1.3f)) * 3.0f;

    if (fplayer->rolleffect < -1.0F) fplayer->rolleffect = -1.0F;
    if (fplayer->rolleffect > 1.0F) fplayer->rolleffect = 1.0F;
//    printf ("%2.2f ", fplayer->rolleffect);
  } 
  else 
    fplayer->rolleffect = 0.0f; 
/*  if (fplayer->rolleffect > 70) fplayer->rolleffect = 70; 
  if (fplayer->rolleffect < -70) fplayer->rolleffect = -70; */

  fplayer->elevatoreffect = dy / height * 2.5; 
  if (fplayer->elevatoreffect > 1.0f) fplayer->elevatoreffect = 1.0f; 
  else if (fplayer->elevatoreffect < -0.5f) fplayer->elevatoreffect = -0.5f; 

#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

const int maxjaxis = 10;
int jaxis [maxjaxis * maxjoysticks];

int getJoystickAxisIndex (int n)
{
  return (n / 1000) * 10 + (n % 1000);
}

void game_joystickaxis (/*int axis1, int axis2, int axis3, int axis4*/)
{
  if (fplayer->ai) return;
/*  int axis [4];
  axis [0] = axis1;
  axis [1] = axis2;
  axis [2] = axis3;
  axis [3] = axis4;*/
  int x = jaxis [getJoystickAxisIndex (joystick_aileron)];
  int y = jaxis [getJoystickAxisIndex (joystick_elevator)];
  int rudder = jaxis [getJoystickAxisIndex (joystick_rudder)];
  int throttle = jaxis [getJoystickAxisIndex (joystick_throttle)];
/*  int t = (int) fplayer->theta;
  if (t < 0) t += 360;
  float rx = x * cosi [t] - y * sine [t];
  float ry = x * sine [t] + y * cosi [t];*/
  float xx = x / 32768.0;
  xx *= fabs (xx);
  fplayer->rolleffect = (float) -xx;
//  fplayer->rectheta -= (float) xx * 6.0F;
/*  fplayer->recgamma += (float) ry / 5000;
  if (fplayer->recgamma > 225) fplayer->recgamma = 225;
  if (fplayer->recgamma < 135) fplayer->recgamma = 135;*/
  fplayer->elevatoreffect = (float) y / 30000;
  if (fplayer->elevatoreffect > 1.0) fplayer->elevatoreffect = 1.0;
  else if (fplayer->elevatoreffect < -1.0) fplayer->elevatoreffect = -1.0;
  if (fplayer->elevatoreffect < -0.5) fplayer->elevatoreffect = -0.5;
  fplayer->ruddereffect = (float) rudder / 30000;
  if (fplayer->ruddereffect > 1.0) fplayer->ruddereffect = 1.0;
  else if (fplayer->ruddereffect < -1.0) fplayer->ruddereffect = -1.0;
//  fplayer->elevatoreffect = 0;
  fplayer->recthrust = fplayer->maxthrust * 0.75 - fplayer->maxthrust / 4 * throttle / 32638;
}

void game_joystickbutton (int button)
{
  if (!fplayer->active) return;
  if (button == joystick_firecannon)
  {
    fplayer->fireCannon (laser);
//    sound->play (SOUND_CANNON1);
#ifdef HAVE_SDL_MIXER
    if (!startcannon)
    {
      sound->playLoop (SOUND_CANNON1);
      startcannon = true;
    }
#else
    sound->play (SOUND_CANNON1);
#endif
  }
  if (button == joystick_firemissile)
  {
    event_fireMissile ();
  }
  if (button == joystick_dropflare)
  {
    event_fireFlare ();
  }
  if (button == joystick_dropchaff)
  {
    event_fireChaff ();
  }
  if (button == joystick_selectmissile)
  {
    event_selectMissile ();
  }
  if (button == joystick_targetnearest)
  {
    event_targetNearest ();
  }
  if (button == joystick_targetlocking)
  {
    event_targetLocking ();
  }
  if (button == joystick_targetnext)
  {
    event_targetNext ();
  }
  if (button == joystick_targetprevious)
  {
    event_targetPrevious ();
  }
  if (button == joystick_thrustup)
  {
    event_thrustUp ();
  }
  if (button == joystick_thrustdown)
  {
    event_thrustDown ();
  }
}

void game_joystickhat (int hat)
{
  int normhat = hat;
#ifndef USE_GLUT
  if (hat % 1000 == SDL_HAT_RIGHT) normhat = 100;
  if (hat % 1000 == SDL_HAT_UP) normhat = 101;
  if (hat % 1000 == SDL_HAT_LEFT) normhat = 102;
  if (hat % 1000 == SDL_HAT_DOWN) normhat = 103;
  normhat += (hat / 1000) * 1000;
  game_joystickbutton (normhat);
#endif
}

int frames = 0, time2 = 0, time1 = 0;

void frame ()
{
  frames ++;
#ifndef USE_GLUT
  time2 = SDL_GetTicks ();
#else
  time2 = glutGet (GLUT_ELAPSED_TIME);
#endif
  if (time2 - time1 > 1000)
  {
    if (time2 - time1 < 1500)
      fps = (float) frames * 1000.0 / (time2 - time1);
    time1 = time2;
    frames = 0;
//    printf ("\nFrames=%f ", fps); fflush (stdout);
  }
}

void game_view ()
{
  frame ();
#ifdef USE_GLUT
  glutSwapBuffers();
#else
  SDL_GL_SwapBuffers ();
#endif
}

int missionending = 0;
int missionstate = 0;

bool missionactive = false;
int menuitem = 0, menutimer = 0, menuitemselected = -1, missionmenutimer;
int missionmenuitemselected = 0, missionmenufighterselected = -1, missionmenuweaponselected = -1;

void drawCircles (CColor *colorstd)
{
  int i;
  float zf = -3, yf, xf;
  int t = 360 - (missionmenutimer * 10 / timestep) % 360;
  for (i = 0; i < 20; i ++)
  {
    yf = -3 + 0.3 * i; // - (float) (missionmenutimer & 63) / 64.0;
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 20; i2 ++)
    {
      xf = -3 + 0.3 * i2;
      yf = -3 + 0.3 * i;
      float cola = sine [(int) (sqrt (xf * xf + yf * yf) * 200 + t) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (xf, yf, zf);
      yf = -3 + 0.3 * (i + 1);
      cola = sine [(int) (sqrt (xf * xf + yf * yf) * 200 + t) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (xf, yf, zf);
    }
    glEnd ();
  }
}

void drawQuads (CColor *colorstd)
{
  int i;
  float zf = -3, yf;
  for (i = 0; i < 14; i ++)
  {
    yf = -3 + 0.5 * i - (float) (missionmenutimer / timestep & 63) / 64.0;
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 14; i2 ++)
    {
      float cola = sine [(i * 100+missionmenutimer*4 / timestep) % 360] / 10 + sine [(i2 * 100) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (-3 + 0.5 * i2, -3 + 0.5 * i, zf + sine [(i * 100) % 360] / 2);
      cola = sine [((i+1) * 100+missionmenutimer*4 / timestep) % 360] / 10 + sine [(i2 * 100) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (-3 + 0.5 * i2, -2.5 + 0.5 * i, zf + sine [((i+1) * 100) % 360] / 2);
    }
    glEnd ();
  }
}

void drawPlasma (CColor *colorstd)
{
  int i;
  float zf = -3, yf;
  float zf1 = -2.98;
  float yind = 200.0 * sin (0.003 * missionmenutimer / timestep) + 200.0;
  float xind = 200.0 * cos (0.003 * missionmenutimer / timestep) + 200.0;
  float yind2 = -200.0 * sin (0.003 * missionmenutimer / timestep) + 200.0;
  float xind2 = -200.0 * cos (0.003 * missionmenutimer / timestep) + 200.0;
  for (i = 0; i < 21; i ++)
  {
    yf = -2.5 + 0.25 * i;
    float yif = yind + i * 5;
    int yi = (int) yif;
    float yr = yif - yi;
    float yif2 = yind2 + i * 5;
    int yi2 = (int) yif2;
    float yr2 = yif2 - yi2;
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 21; i2 ++)
    {
      float xf = -2.5 + 0.25 * i2;
      float xif = xind + i2 * 5;
      int xi = (int) xif;
      float xr = xif - xi;
      float xif2 = xind2 + i2 * 5;
      int xi2 = (int) xif2;
      float xr2 = xif2 - xi2;
      float h1 = xr * yr * l->h [yi + 1] [xi + 1] + (1.0 - xr) * (1.0 - yr) * l->h [yi] [xi] +
                 xr * (1.0 - yr) * l->h [yi] [xi + 1] + (1.0 - xr) * yr * l->h [yi + 1] [xi];
      float h2 = xr * yr * l->h [yi + 1 + 5] [xi + 1] + (1.0 - xr) * (1.0 - yr) * l->h [yi + 5] [xi] +
                 xr * (1.0 - yr) * l->h [yi + 5] [xi + 1] + (1.0 - xr) * yr * l->h [yi + 1 + 5] [xi];
      float h3 = xr2 * yr2 * l->h [yi2 + 1] [xi2 + 1] + (1.0 - xr2) * (1.0 - yr2) * l->h [yi2] [xi2] +
                 xr2 * (1.0 - yr2) * l->h [yi2] [xi2 + 1] + (1.0 - xr2) * yr2 * l->h [yi2 + 1] [xi2];
      float h4 = xr2 * yr2 * l->h [yi2 + 1 + 5] [xi2 + 1] + (1.0 - xr2) * (1.0 - yr2) * l->h [yi2 + 5] [xi2] +
                 xr2 * (1.0 - yr2) * l->h [yi2 + 5] [xi2 + 1] + (1.0 - xr2) * yr2 * l->h [yi2 + 1 + 5] [xi2];
      float intens = sin (0.15 * (h1 / 256 + 0.5 * missionmenutimer / timestep)) * 0.14 + 0.14;
      float intens2 = sin (0.15 * (h3 / 256 + 0.5 * missionmenutimer / timestep)) * 0.06 + 0.06;
      if (colorstd == &colorblue) glColor3f (intens2, 0, intens);
      else glColor3f (intens, 0, intens2);
      glVertex3f (xf, yf, zf1);
      intens = sin (0.15 * (h2 / 256 + 0.5 * missionmenutimer / timestep)) * 0.16 + 0.16;
      intens2 = sin (0.15 * (h4 / 256 + 0.5 * missionmenutimer / timestep)) * 0.08 + 0.08;
      if (colorstd == &colorblue) glColor3f (intens2, 0, intens);
      else glColor3f (intens, 0, intens2);
      glVertex3f (xf, yf + 0.25, zf1);
    }
    glEnd ();
  }
}

void menu_key (unsigned char key, int x, int y)
{
  if (pilotedit.active)
  {
    pilotedit.event (key);
    if (key == 13)
      pilots->add (pilotedit.text);
    key = 0;
  }
  if (key == 27)
  {
    if (missionactive)
    {
      switch_game ();
    }
    else
    {
      switch_quit ();
    }
  }
}

void pleaseWait ()
{
  float xf = 1.0, yf = 0.2, zf = -2.5;
  int c1 = 100, c2 = 10, c3 = 180;
  glBegin (GL_QUADS);
  glColor4ub (c2, c2, c2, 255);
  glVertex3f (-xf, -yf, zf);
  glColor4ub (c1, c1, c1, 255);
  glVertex3f (xf, -yf, zf);
  glColor4ub (c2, c2, c2, 255);
  glVertex3f (xf, yf, zf);
  glColor4ub (c1, c1, c1, 255);
  glVertex3f (-xf, yf, zf);
  glEnd ();
  glBegin (GL_LINE_STRIP);
  glColor4ub (c3, c3, c3, 255);
  glVertex3f (-xf, -yf, zf);
  glVertex3f (xf, -yf, zf);
  glVertex3f (xf, yf, zf);
  glVertex3f (-xf, yf, zf);
  glVertex3f (-xf, -yf, zf);
  glEnd ();

  font1->drawTextCentered (0, -0.5, -1.5, "PLEASE WAIT...", &colorwhite);
  game_view ();
}

void stats_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    pleaseWait ();
    display ("Initing new mission", LOG_MOST);
    missionactive = false;
    createMission (MISSION_DEMO);
    game_levelInit ();
    sound->haltMusic ();
    switch_menu ();
  }
}

void credits_key (unsigned char key, int x, int y)
{
  if (key)
  {
//    sound->haltMusic ();
    switch_menu ();
  }
}

void finish_key (unsigned char key, int x, int y)
{
  if (key)
  {
//    sound->haltMusic ();
    switch_menu ();
  }
}

void mission_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    switch_menu ();
  }
  else if (key == 13)
  {
    pleaseWait ();
    game_levelInit ();
    switch_game ();
    missionactive = true;
  }
}

void fame_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    switch_menu ();
  }
}

void create_key (unsigned char key, int x, int y)
{
  if (key == 's')
  {
//    server->sendMessage (0, "s", 1);
    if (server->checkStart()) 
    {
    printf ("Starting..");
    createMission (MISSION_MULTIPLAYER_DOGFIGHT);
    game_levelInit ();
    switch_game ();
    missionactive = true;
    }
  }
  if (key == 27)
  {
    switch_menu ();
  }
}

void join_key (unsigned char key, int x, int y)
{
  if (key == 's')
  {
    createMission (MISSION_MULTIPLAYER_DOGFIGHT);
    game_levelInit ();
    switch_game ();
    missionactive = true;
  }
  if (key == 27)
  {
    switch_menu ();
  }
}

void fighter_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    switch_menu ();
  }
}

char *getModelText (int id)
{
  if (id == FIGHTER_FALCON)
    return "THE FALCON IS A NIMBLE AND\nFAST FIGHTER PROVIDING\nBOTH AIR-AIR AND\nAIR-GROUND-MISSILES.";
  else if (id == FIGHTER_HAWK)
    return "THE HAWK IS A TYPICAL\nALLROUNDER AND DESIGNED\nFOR GROUND ATTACKS.";
  else if (id == FIGHTER_HAWK2)
    return "THE HAWK II IS A ...";
  return "NO INFO";
}

char *getModelName (int id)
{
  if (id == FIGHTER_FALCON)
    return "GL-16 FALCON";
  else if (id == FIGHTER_HAWK)
    return "GL-22 HAWK";
  else if (id == FIGHTER_HAWK2)
    return "GL-23 HAWK II";
  else if (id == FIGHTER_REDARROW)
    return "GL-15 REDARROW";
  else if (id == FIGHTER_PHOENIX)
    return "GL-117 PHOENIX";
  else if (id == FIGHTER_CROW)
    return "CROW";
  else if (id == FIGHTER_BUZZARD)
    return "BUZZARD";
  else if (id == FIGHTER_SWALLOW)
    return "SWALLOW";
  else if (id == FIGHTER_BLACKBIRD)
    return "BLACKBIRD";
  else if (id == MISSILE_AIR1)
    return "AIR-AIR MISSILE PACK";
  else if (id == MISSILE_GROUND1)
    return "AIR-GROUND MISSILE PACK";
  else if (id == MISSILE_DF1)
    return "DUMB FIRE MISSILE PACK";
  return "NO INFO";
}

void mission_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  int lastitemselected = missionmenuitemselected;
//  int lastitem = menuitem;
  missionmenuitemselected = -1;

  if (ry >= 0.88 && ry <= 0.93)
  {
    if (rx >= 0.55 && rx <= 0.85)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        pleaseWait ();
        game_levelInit ();
        switch_game ();
        missionactive = true;
      }
    }
    if (rx >= 0.12 && rx <= 0.35)
    {
      missionmenuitemselected = 1;
      if (state == MOUSE_DOWN)
      {
        switch_menu ();
      }
    }
  }

  missionmenufighterselected = -1;
  if (ry >= 0.55 && ry <= 0.75)
  {
    if (rx >= 0.1 && rx < 0.25)
    {
      missionmenufighterselected = 0;
      if (state == MOUSE_DOWN)
        missionnew->wantfighter = 0;
    }
    if (rx >= 0.25 && rx < 0.4 && missionnew->selfighters >= 2)
    {
      missionmenufighterselected = 1;
      if (state == MOUSE_DOWN)
        missionnew->wantfighter = 1;
    }
    if (rx >= 0.4 && rx < 0.55 && missionnew->selfighters >= 3)
    {
      missionmenufighterselected = 2;
      if (state == MOUSE_DOWN)
        missionnew->wantfighter = 2;
    }
  }

  missionmenuweaponselected = -1;
  if (ry >= 0.55 && ry <= 0.75)
  {
    if (rx >= 0.6 && rx < 0.7)
    {
      missionmenuweaponselected = 0;
      if (state == MOUSE_DOWN)
        missionnew->wantweapon = 0;
    }
    if (rx >= 0.7 && rx < 0.8 && missionnew->selweapons >= 2)
    {
      missionmenuweaponselected = 1;
      if (state == MOUSE_DOWN)
        missionnew->wantweapon = 1;
    }
    if (rx >= 0.8 && rx < 0.9 && missionnew->selweapons >= 3)
    {
      missionmenuweaponselected = 2;
      if (state == MOUSE_DOWN)
        missionnew->wantweapon = 2;
    }
  }

  if (lastitemselected != missionmenuitemselected)
  {
//    missionmenutimer = 0; // do not alter, it will affect the background
  }
}

void drawArrow (float x, float y, float w, float h)
{
  float zf = -3;
  gl->enableTextures (texarrow->textureID);
  gl->enableAlphaBlending ();
  glBegin (GL_QUADS);
  glColor3ub (180, 180, 180);

  if (w > h)
  {
    glTexCoord2f (0, 1);
    glVertex3f (x, y, zf);
    glTexCoord2f (0, 0);
    glVertex3f (x + w, y, zf);
    glTexCoord2f (1, 0);
    glVertex3f (x + w, y + h, zf);
    glTexCoord2f (1, 1);
    glVertex3f (x, y + h, zf);
  }
  else
  {
    glTexCoord2f (1, 0);
    glVertex3f (x, y - h, zf);
    glTexCoord2f (1, 1);
    glVertex3f (x, y, zf);
    glTexCoord2f (0, 1);
    glVertex3f (x + w, y, zf);
    glTexCoord2f (0, 0);
    glVertex3f (x + w, y - h, zf);
  }

  glEnd ();
  glDisable (GL_BLEND);
  glDisable (GL_TEXTURE_2D);
}

void mission_display ()
{
  char buf [256];
  int i;
  float zf = -3;
  float xstats = 1, ystats = 6;
  float xstatstab = 10;
  float piloty = 6;
  Pilot *p = pilots->pilot [pilots->aktpilot];
  CColor *colorstd = &colorred;
  if (p->mission_state [missionnew->id] == 1)
    colorstd = &colorblue;

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  gl117_rotateColors (missionmenutimer);

  drawPlasma (colorstd);

  drawArrow (-2.1, 1.6, 1, 0.05);
  drawArrow (-2.3, 1.6, 0.05, 0.6);

  drawArrow (-2.1, 0.8, 1, 0.05);
  drawArrow (-2.3, 0.8, 0.05, 0.9);

  drawArrow (0.2, 0.8, 1, 0.05);
  drawArrow (0, 0.8, 0.05, 0.9);

  drawArrow (-2.1, -0.3, 1, 0.05);
  drawArrow (-2.3, -0.3, 0.05, 0.9);

  drawArrow (0.2, -0.3, 1, 0.05);
  drawArrow (0, -0.3, 0.05, 0.9);

/*  glLineWidth (2.0);
  glColor4ub (180, 180, 180, 255);
  glBegin (GL_LINE_STRIP);
  glVertex3f (-2.3, 0.8, zf);
  glVertex3f (2.3, 0.8, zf);
  glVertex3f (2.3, 1.6, zf);
  glVertex3f (-2.3, 1.6, zf);
  glVertex3f (-2.3, 0.8, zf);
  glEnd ();
  glBegin (GL_LINE_STRIP);
  glVertex3f (-2.3, -0.3, zf);
  glVertex3f (2.3, -0.3, zf);
  glVertex3f (2.3, 0.8, zf);
  glVertex3f (-2.3, 0.8, zf);
  glVertex3f (-2.3, -0.3, zf);
  glEnd ();
  glBegin (GL_LINE_STRIP);
  glVertex3f (-2.3, -1.5, zf);
  glVertex3f (2.3, -1.5, zf);
  glVertex3f (2.3, -0.3, zf);
  glVertex3f (-2.3, -0.3, zf);
  glVertex3f (-2.3, -1.5, zf);
  glEnd ();
  glBegin (GL_LINE_STRIP);
  glVertex3f (0, 0.8, zf);
  glVertex3f (0, -1.5, zf);
  glEnd ();*/

  CVector3 vec;
  CVector3 tl (-4.5, 5.5, -8.0);
  CRotation rot;
  rot.a = 270;
  rot.b = (270 - missionmenutimer * 4 / timestep) % 360;
  rot.c = 90;
  
  // Draw dummy missile
  glEnable (GL_LIGHTING);
  model_missile1.draw (&vec, &tl, &rot, 0.05, 1.0, 0);
  glDisable (GL_LIGHTING);
  
  glEnable (GL_LIGHTING);
  glEnable (GL_DEPTH_TEST);
  model_gl117.draw2 (&vec, &tl, &rot, 1.0, 0);
  tl.x = 4.5;
  model_gl117.draw2 (&vec, &tl, &rot, 1.0, 0);
  
  tl.x = -20; tl.y = -2.4; tl.z = -8.5;
  rot.a = 245;
  rot.b = 0;
  for (i = 0; i < missionnew->selfighters; i ++)
  {
    tl.x = -5 + (float) i * 2.3;
    if (missionnew->wantfighter == i)
      rot.c = (5 + missionmenutimer * 4 / timestep) % 360;
    else
      rot.c = 5;
    getModel (missionnew->selfighter [i])->draw2 (&vec, &tl, &rot, 0.9, 0);
  }

  tl.x = 0; tl.y = -2.4; tl.z = -8.5;
  rot.a = 245;
  rot.b = 0;
  for (i = 0; i < missionnew->selweapons; i ++)
  {
    tl.x = 2.0 + (float) i * 1.5;
    if (missionnew->wantweapon == i)
      rot.c = (5 + missionmenutimer * 4 / timestep) % 360;
    else
      rot.c = 5;
    getModel (missionnew->selweapon [i])->draw2 (&vec, &tl, &rot, 0.9, 0);
  }
  glDisable (GL_DEPTH_TEST);
  glDisable (GL_LIGHTING);

  font1->drawTextCentered (0, 9.8, -1.5, missionnew->name);
  font1->drawText (-22, 14, -3, "BRIEFING:");
  font2->drawText (-22, 12, -3, missionnew->briefing);

  font1->drawText (xstats, ystats, -3, "STATUS:");
  if (p->mission_state [missionnew->id] == 1)
    font1->drawText (xstatstab, ystats, -3, "SUCCESS", colorstd);
  else if (p->mission_state [missionnew->id] == 2)
    font1->drawText (xstatstab, ystats, -3, "FAILED", colorstd);
  else
    font1->drawText (xstatstab, ystats, -3, "EMPTY", colorstd);
  if (missionnew->id >= MISSION_CAMPAIGN1 && missionnew->id <= MISSION_CAMPAIGN2)
  {
    font1->drawText (xstats, ystats - 1.5, -3, "SCORE:");
    int score = p->mission_score [missionnew->id];
    if (score < -10000 || score > 100000) score = 0;
    sprintf (buf, "%d", score);
    font1->drawText (xstatstab, ystats - 1.5, -3, buf, colorstd);
    font1->drawText (xstats, ystats - 3, -3, "KILLS:");
    sprintf (buf, "%d AIRCRAFTS", p->mission_fighterkills [missionnew->id]);
    font1->drawText (xstatstab, ystats - 3, -3, buf, colorstd);
    drawMedal (xstatstab + 2, ystats - 5.5, -3, getMedal (p->mission_score [missionnew->id]), 3.5, mission->id);
  }
  else
  {
    font1->drawText (xstats, ystats - 1.5, -3, "SCORE:");
    font1->drawText (xstatstab, ystats - 1.5, -3, "TRAINING");
  }
  
  font1->drawText (-22, piloty, -3, "PILOTS:");
  strcpy (buf, pilots->pilot [pilots->aktpilot]->getRank ());
  strcat (buf, " ");
  strcat (buf, pilots->pilot [pilots->aktpilot]->name);
  font2->drawText (-20, piloty - 2 + 0.2, -3, buf);
  drawRank (-22, piloty - 2 + 0.3, -3, pilots->pilot [pilots->aktpilot]->ranking, 0.8);
  for (i = 1; i < missionnew->alliedfighters; i ++)
    if (missionnew->alliedpilot [i - 1] >= 0 && missionnew->alliedpilot [i - 1] < 100)
    {
      drawRank (-22, piloty - 2 - i + 0.1, -3, pilots->pilot [pilots->aktpilot]->tp [missionnew->alliedpilot [i - 1]]->ranking, 0.8);
      font2->drawText (-20, piloty - 2 - i, -3, pilots->pilot [pilots->aktpilot]->tp [missionnew->alliedpilot [i - 1]]->getName ());
    }

  font1->drawText (-22, -5, -3, "CHOOSE FIGHTER:");
  font1->drawText (xstats, -5, -3, "CHOOSE WEAPON PACK:");
  if (missionmenuitemselected == 0)
    font1->drawTextScaled (2, -14, -2, "START MISSION", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (2, -14, -2, "START MISSION");
  if (missionmenuitemselected == 1)
    font1->drawTextScaled (-12, -14, -2, "CANCEL", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-12, -14, -2, "CANCEL");
  font2->drawText (-20, -14, -3, getModelName (missionnew->selfighter [missionnew->wantfighter]));
  font2->drawText (xstats + 2, -14, -3, getModelName (missionnew->selweapon [missionnew->wantweapon]));
  glPopMatrix ();

  adjustBrightness ();

  drawMouseCursor ();
}

void fame_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  if (ry >= 0.75 && ry <= 0.82)
  {
    if (rx >= 0.4 && rx <= 0.6)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        fame_key (27, 0, 0);
      }
    }
  }
}

void create_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  if (ry >= 0.8 && ry <= 0.9)
  {
    if (rx >= 0.4 && rx <= 0.6)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        create_key (27, 0, 0);
      }
    }
  }
}

void create_display ()
{
  int i;
  CColor *colorstd = &colorblue;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  drawQuads (colorstd);

  float my = 0;
  for (i = 0; i < server->num_clients; i ++)
  {
    font1->drawTextCentered (0, my, -2.5, server->clients [i].name);
    my -= 2;
  }

  font1->drawTextCentered (0, 9, -1.5, "CREATE GAME");

  if (missionmenuitemselected == 0)
    font1->drawTextScaled (-2, -12, -2, "BACK", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-2, -12, -2, "BACK");

  glPopMatrix ();

  adjustBrightness ();

  drawMouseCursor ();
}

void join_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  if (ry >= 0.8 && ry <= 0.9)
  {
    if (rx >= 0.4 && rx <= 0.6)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        create_key (27, 0, 0);
      }
    }
  }
}

void join_display ()
{
  CColor *colorstd = &colorblue;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  drawQuads (colorstd);

  font1->drawTextCentered (0, 9, -1.5, "JOIN GAME");

  if (missionmenuitemselected == 0)
    font1->drawTextScaled (-2, -12, -2, "BACK", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-2, -12, -2, "BACK");

  glPopMatrix ();

  adjustBrightness ();

  drawMouseCursor ();
}

int aktfighter = 0;

void fighter_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  int maxfighter = 5;
  Pilot *p = pilots->pilot [pilots->aktpilot];
  if (p->mission_state [MISSION_BASE] == 1) maxfighter ++;
  if (p->mission_state [MISSION_SHIP1] == 1) maxfighter ++;
  if (p->mission_state [MISSION_CANYON3] == 1) maxfighter ++;
  if (p->mission_state [MISSION_MOON1] == 1) maxfighter ++;

  if (ry >= 0.45 && ry <= 0.55)
  {
    if (rx >= 0.08 && rx <= 0.18)
    {
      missionmenuitemselected = 10;
      if (state == MOUSE_DOWN)
      {
        aktfighter --;
        if (aktfighter < 0) aktfighter = maxfighter - 1;
      }
    }
    if (rx >= 0.82 && rx <= 0.92)
    {
      missionmenuitemselected = 11;
      if (state == MOUSE_DOWN)
      {
        aktfighter ++;
        if (aktfighter >= maxfighter) aktfighter = 0;
      }
    }
  }

  if (ry >= 0.8 && ry <= 0.9)
  {
    if (rx >= 0.4 && rx <= 0.6)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        fighter_key (27, 0, 0);
      }
    }
  }
}

void fighter_display ()
{
  char buf [256];
//  Pilot *p = pilots->pilot [pilots->aktpilot];
  CColor *colorstd = &colorblue;

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  drawQuads (colorstd);

  font1->drawTextCentered (0, 9, -1.5, "FIGHTER");

  glBegin (GL_TRIANGLES);
  if (missionmenuitemselected == 10)
    glColor3ub (220, 220, 0);
  else
    glColor3ub (150, 150, 150);
  glVertex3f (-1.3, 0, -2);
  glVertex3f (-1.1, -0.1, -2);
  glVertex3f (-1.1, 0.1, -2);
  glEnd ();  
  glBegin (GL_LINE_STRIP);
  glColor3ub (200, 200, 200);
  glVertex3f (-1.3, 0, -2);
  glVertex3f (-1.1, -0.1, -2);
  glVertex3f (-1.1, 0.1, -2);
  glVertex3f (-1.3, 0, -2);
  glEnd ();
  glBegin (GL_TRIANGLES);
  if (missionmenuitemselected == 11)
    glColor3ub (220, 220, 0);
  else
    glColor3ub (150, 150, 150);
  glVertex3f (1.3, 0, -2);
  glVertex3f (1.1, -0.1, -2);
  glVertex3f (1.1, 0.1, -2);
  glEnd ();  
  glBegin (GL_LINE_STRIP);
  glColor3ub (200, 200, 200);
  glVertex3f (1.3, 0, -2);
  glVertex3f (1.1, -0.1, -2);
  glVertex3f (1.1, 0.1, -2);
  glVertex3f (1.3, 0, -2);
  glEnd ();


  CVector3 vec;
  CVector3 tl;
  tl.y = -0.4;
  tl.z = -3;
  CRotation rot;
  rot.a = 310;
  rot.b = 0;
  rot.c = (5 + missionmenutimer * 4 / timestep) % 360;
  CModel *model;
  int id;
  if (aktfighter == 0) { model = &model_fig; id = FIGHTER_FALCON; }
  else if (aktfighter == 1) { model = &model_fige; id = FIGHTER_CROW; }
  else if (aktfighter == 2) { model = &model_figb; id = FIGHTER_HAWK; }
  else if (aktfighter == 3) { model = &model_figa; id = FIGHTER_SWALLOW; }
  else if (aktfighter == 4) { model = &model_figd; id = FIGHTER_BUZZARD; }
  else if (aktfighter == 5) { model = &model_figc; id = FIGHTER_HAWK2; }
  else if (aktfighter == 6) { model = &model_figg; id = FIGHTER_REDARROW; }
  else if (aktfighter == 7) { model = &model_figf; id = FIGHTER_PHOENIX; }
  else { model = &model_figh; id = FIGHTER_BLACKBIRD; }

/* Some code to get screenshots of missiles
glClearColor (1, 1, 1, 1);
gl->clearScreen ();
model = &model_missile7; */

  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  model->draw (&vec, &tl, &rot, 1.0, 1.0, 0);
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);

  fplayer->newinit (id, 1, 0);

  font1->drawText (-10, 9, -2, getModelName (id));
  float yf = 9.5;
  strcpy (buf, "TYPE: ");
  if (fplayer->id == FIGHTER_FALCON || fplayer->id == FIGHTER_CROW || fplayer->id == FIGHTER_BUZZARD || fplayer->id == FIGHTER_REDARROW || fplayer->id == FIGHTER_BLACKBIRD)
    strcat (buf, "FIGHTER");
  else
    strcat (buf, "BOMBER");
  font1->drawText (-10, yf, -2.5, buf);
  yf -= 1.5;
  strcpy (buf, "SPEED: ");
  if (fplayer->maxthrust < 0.20) strcat (buf, "VERY SLOW");
  else if (fplayer->maxthrust < 0.25) strcat (buf, "SLOW");
  else if (fplayer->maxthrust < 0.3) strcat (buf, "INTERMEDITATE");
  else if (fplayer->maxthrust < 0.33) strcat (buf, "FAST");
  else strcat (buf, "EXTREMELY FAST");
  font1->drawText (-10, yf, -2.5, buf);
  yf -= 1.5;
  strcpy (buf, "MANOEVERABILITY: ");
  if (fplayer->manoeverability <= 0.25) strcat (buf, "VERY BAD");
  else if (fplayer->manoeverability <= 0.33) strcat (buf, "BAD");
  else if (fplayer->manoeverability <= 0.42) strcat (buf, "INTERMEDITATE");
  else if (fplayer->manoeverability <= 0.5) strcat (buf, "HIGH");
  else strcat (buf, "EXTREMELY HIGH");
  font1->drawText (-10, yf, -2.5, buf);
  yf -= 1.5;
  strcpy (buf, "SHIELD: ");
  if (fplayer->maxshield <= 70) strcat (buf, "VERY LOW");
  else if (fplayer->maxshield <= 100) strcat (buf, "LOW");
  else if (fplayer->maxshield <= 130) strcat (buf, "INTERMEDITATE");
  else if (fplayer->maxshield <= 160) strcat (buf, "STRONG");
  else strcat (buf, "EXTREMELY STRONG");
  font1->drawText (-10, yf, -2.5, buf);

  if (missionmenuitemselected == 0)
    font1->drawTextScaled (-2, -12, -2, "BACK", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-2, -12, -2, "BACK");

  glPopMatrix ();

  adjustBrightness ();

  drawMouseCursor ();
}

void fame_display ()
{
  char buf [256];
  int i, i2;
  Pilot *p = pilots->pilot [pilots->aktpilot];
  CColor *colorstd = &colorblue;

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  drawCircles (colorstd);

  font1->drawTextCentered (0, 8, -1.5, "TOP PILOTS");
  int sum = 0;
  for (i = MISSION_CAMPAIGN1; i < MISSION_CAMPAIGN2; i ++)
    sum += p->mission_fighterkills [i];
  p->tp [10]->fighterkills = sum;
  strcpy (p->tp [10]->name, p->name);
  p->tp [10]->ranking = p->ranking;
  int index [11];
  for (i = 0; i < 11; i ++)
    index [i] = i;
  for (i = 0; i < 11; i ++)
    for (i2 = 0; i2 < i; i2 ++)
    {
      if (p->tp [index [i]]->fighterkills < p->tp [index [i2]]->fighterkills)
      {
        int temp = index [i];
        index [i] = index [i2];
        index [i2] = temp;
      }
    }

  for (i = 0; i < 11; i ++)
  {
    drawRank (-12.5, 1.5 * i - 7, -2.5, p->tp [index [i]]->ranking, 1);
    sprintf (buf, "%s %s", p->tp [index [i]]->getRank (), p->tp [index [i]]->name);
    font1->drawText (-10, 1.5 * i - 7, -2.5, buf);
    sprintf (buf, "%d", p->tp [index [i]]->fighterkills);
    font1->drawText (11, 1.5 * i - 7, -2.5, buf);
  }

  if (missionmenuitemselected == 0)
    font1->drawTextScaled (-2, -10, -2, "BACK", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-2, -10, -2, "BACK");

  glPopMatrix ();

  adjustBrightness ();

  drawMouseCursor ();
}

CModel *explsphere;
CSphere *mysphere;

void game_quit ()
{
  int i;
  volumesound = sound->volumesound;
  volumemusic = sound->volumemusic;
  save_config ();
  save_configInterface ();
  pilots->save (dirs->getSaves ("pilots"));
  display ("Pilots saved", LOG_MOST);
/*  for (i = 0; i < maxfighter; i ++)
    delete (fighter [i]);*/
  for (i = 0; i < maxlaser; i ++)
    delete (laser [i]);
  for (i = 0; i < maxmissile; i ++)
    delete (missile [i]);
  for (i = 0; i < maxflare; i ++)
    delete (flare [i]);
  for (i = 0; i < maxchaff; i ++)
    delete (chaff [i]);
  for (i = 0; i < maxexplosion; i ++)
    delete (explosion [i]);
  for (i = 0; i < maxstar; i ++)
    delete (star [i]);
  for (i = 0; i < maxgroundobj; i ++)
    delete (groundobj [i]);
  for (i = 0; i < maxblacksmoke; i ++)
    delete (blacksmoke [i]);
  delete pilots;
  delete tlinf;
  delete tlminf;
  delete tlnull;
  delete explsphere;
  delete objsphere;
  delete sphere;
  delete flash1;
  delete cockpit;
  delete font1;
  delete font2;
  delete space;
  delete dirs;
  delete gl;
#ifndef USE_GLUT
//  SDL_CloseAudio();
//  SDL_FreeWAV(wave.sound);
//  SDL_Quit (); // done atexit()
  delete sound;
#endif
//  fprintf (stdout, "\n"); fflush (stdout);
  exit (EXIT_NORMAL);
}

void quit_key (unsigned char key, int x, int y)
{
  if (key == 'y' || key == 'Y')
    game_quit ();
  else
    switch_menu ();
}

void quit_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  if (ry >= 0.5 && ry <= 0.55)
  {
    if (rx >= 0.35 && rx <= 0.45)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        quit_key ('y', 0, 0);
      }
    }
    if (rx >= 0.55 && rx <= 0.65)
    {
      missionmenuitemselected = 1;
      if (state == MOUSE_DOWN)
      {
        quit_key (27, 0, 0);
      }
    }
  }
}

void menu_mouse (int button, int state, int x, int y)
{
  int i;
  float rx = (float) x / width;
  float ry = (float) y / height;
  int lastitemselected = menuitemselected;
  int lastitem = menuitem;
  menuitemselected = -1;

  if (rx >= 0.08 && rx <= 0.4)
  {
    if (ry >= 0.15 && ry <= 0.2)
    {
      menuitemselected = 0;
      if (state == MOUSE_DOWN)
        menuitem = 0;
    }
    else if (ry >= 0.22 && ry <= 0.27)
    {
      menuitemselected = 1;
      if (state == MOUSE_DOWN)
        menuitem = 1;
    }
    else if (ry >= 0.28 && ry <= 0.33)
    {
      menuitemselected = 2;
      if (state == MOUSE_DOWN)
        menuitem = 2;
    }
    else if (ry >= 0.34 && ry <= 0.39)
    {
      menuitemselected = 3;
      if (state == MOUSE_DOWN)
        menuitem = 3;
    }
    else if (ry >= 0.40 && ry <= 0.45)
    {
      menuitemselected = 4;
      if (state == MOUSE_DOWN)
        menuitem = 4;
    }
    else if (ry >= 0.46 && ry <= 0.51)
    {
      menuitemselected = 5;
      if (state == MOUSE_DOWN)
      {
        menuitem = 5;
        switch_credits ();
      }
    }
    else if (ry >= 0.52 && ry <= 0.57)
    {
      menuitemselected = 6;
      if (state == MOUSE_DOWN)
      {
        menuitem = 6;
        switch_quit ();
      }
    }
#ifdef HAVE_SDL_NET
    else if (ry >= 0.58 && ry <= 0.63)
    {
      menuitemselected = 7;
      if (state == MOUSE_DOWN)
        menuitem = 7;
    }
#endif
    else if (ry >= 0.64 && ry <= 0.69)
    {
      menuitemselected = 9;
      if (state == MOUSE_DOWN)
      {
        // no new menuitem!
        switch_game ();
      }
    }
  }

  if (menuitem == 0)
  {
    if (rx >= 0.48 && rx <= 0.85)
    {
      for (i = 0; i < pilots->aktpilots; i ++)
      {
        if (ry >= 0.22 + 0.06 * (float) i && ry <= 0.27 + 0.06 * (float) i)
        {
          menuitemselected = 20 + i;
          if (state == MOUSE_DOWN)
          {
            pilots->aktpilot = i;
          }
        }
      }
      if (ry >= 0.58 && ry <= 0.63)
      {
        menuitemselected = 10;
        if (state == MOUSE_DOWN)
          if (button == MOUSE_BUTTON_RIGHT)
          {
            pilots->rm ();
          }
      }
      if (ry >= 0.64 && ry <= 0.69)
      {
        menuitemselected = 11;
        if (state == MOUSE_DOWN)
        {
          pilotedit.init ();
        }
      }
    }
  }

  if (menuitem == 1)
  {
//    Pilot *p = pilots->pilot [pilots->aktpilot];
    if (rx >= 0.48 && rx <= 0.85)
    {
      if (ry >= 0.12 && ry <= 0.14)
      {
        menuitemselected = 10;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_TUTORIAL);
        }
      }
      if (ry >= 0.15 && ry <= 0.17)
      {
        menuitemselected = 11;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_DOGFIGHT);
        }
      }
      if (ry >= 0.21 && ry <= 0.23)
      {
        menuitemselected = 13;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_FREEFLIGHT1);
        }
      }
      if (ry >= 0.24 && ry <= 0.26)
      {
        menuitemselected = 14;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_DEATHMATCH1);
        }
      }
      if (ry >= 0.27 && ry <= 0.29)
      {
        menuitemselected = 15;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_DEATHMATCH2);
        }
      }
      if (ry >= 0.30 && ry <= 0.32)
      {
        menuitemselected = 16;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_TEAMBASE1);
        }
      }
      if (ry >= 0.33 && ry <= 0.35)
      {
        menuitemselected = 17;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_WAVES1);
        }
      }
    }
  }

  if (menuitem == 2)
  {
    Pilot *p = pilots->pilot [pilots->aktpilot];
    if (rx >= 0.48 && rx <= 0.85)
    {
      float ryu = 0.12F;
      if (ry >= ryu && ry <= ryu + 0.02F)
      {
        menuitemselected = 10;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_TEST1);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_TEST1] == 1)
      {
        menuitemselected = 11;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_TEST2);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_TEST2] == 1)
      {
        menuitemselected = 12;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_TRANSPORT);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_TRANSPORT] == 1)
      {
        menuitemselected = 13;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_CONVOY);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_CONVOY] == 1)
      {
        menuitemselected = 14;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_DOGFIGHT2);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_DOGFIGHT2] == 1)
      {
        menuitemselected = 15;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_AIRBATTLE);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_AIRBATTLE] == 1)
      {
        menuitemselected = 16;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_SADEFENSE);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_SADEFENSE] == 1)
      {
        menuitemselected = 17;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_SCOUT);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_SCOUT] == 1)
      {
        menuitemselected = 18;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_BASE);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_BASE] == 1)
      {
        menuitemselected = 20;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_DEFEND1);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_DEFEND1] == 1)
      {
        menuitemselected = 21;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_DOGFIGHT3);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_DOGFIGHT3] == 1)
      {
        menuitemselected = 22;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_TANK1);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_TANK1] == 1)
      {
        menuitemselected = 25;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_SHIP1);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_SHIP1] == 1)
      {
        menuitemselected = 26;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_SHIP2);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_SHIP2] == 1)
      {
        menuitemselected = 27;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_SHIP3);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_SHIP3] == 1)
      {
        menuitemselected = 30;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_CANYON1);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_CANYON1] == 1)
      {
        menuitemselected = 31;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_CANYON2);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_CANYON2] == 1)
      {
        menuitemselected = 36;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_TUNNEL1);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_TUNNEL1] == 1)
      {
        menuitemselected = 32;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_CANYON3);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_CANYON3] == 1)
      {
        menuitemselected = 33;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_MOON1);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_MOON1] == 1)
      {
        menuitemselected = 34;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_MOON2);
        }
      }
      ryu += 0.03F;
      if (ry >= ryu && ry <= ryu + 0.02F && p->mission_state [MISSION_MOON2] == 1)
      {
        menuitemselected = 35;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_MOON3);
        }
      }
    }
    if (ry >= 0.82 && ry <= 0.88)
    {
      if (rx >= 0.40 && rx <= 0.6)
      {
        menuitemselected = 100;
        if (state == MOUSE_DOWN)
        {
          switch_fame ();
        }
      }
      if (rx >= 0.65 && rx <= 0.85)
      {
        menuitemselected = 101;
        if (state == MOUSE_DOWN)
        {
          switch_fighter ();
        }
      }
    }
  }

  if (menuitem == 3)
  {
    if (rx >= 0.48 && rx <= 0.85)
    {
      if (ry >= 0.15 && ry <= 0.2)
      {
        menuitemselected = 10;
        if (state == MOUSE_DOWN)
        {
          if (button == MOUSE_BUTTON_LEFT)
          {
            quality ++;
            if (quality > 5) quality = 0;
          }
          else
          {
            quality --;
            if (quality < 0) quality = 5;
          }
        }
      }
      else if (ry >= 0.22 && ry <= 0.27)
      {
        menuitemselected = 11;
        if (state == MOUSE_DOWN)
        {
          if (button == MOUSE_BUTTON_LEFT)
          {
            view += 10;
            if (view > 100) view = 30;
            menu_reshape ();
          }
          else
          {
            view -= 10;
            if (view < 30) view = 100;
            menu_reshape ();
          }
        }
      }
      else if (ry >= 0.28 && ry <= 0.33)
      {
        menuitemselected = 12;
        if (state == MOUSE_DOWN)
        {
          dithering = (dithering == 0 ? 1 : 0);
        }
      }
      else if (ry >= 0.34 && ry <= 0.39)
      {
        menuitemselected = 13;
        if (state == MOUSE_DOWN)
        {
          if (button == MOUSE_BUTTON_LEFT)
          {
            fplayer->rolleffect = 0;
            fplayer->ruddereffect = 0;
            fplayer->elevatoreffect = 0;
            if (controls == CONTROLS_KEYBOARD) controls = CONTROLS_MOUSE;
            else if (controls == CONTROLS_MOUSE) controls = CONTROLS_MOUSE_REVERSE;
            else if (controls == CONTROLS_MOUSE_REVERSE) controls = CONTROLS_MOUSE_EXP;
            else if (controls == CONTROLS_MOUSE_EXP) controls = CONTROLS_JOYSTICK;
            else if (controls == CONTROLS_JOYSTICK) controls = CONTROLS_MOUSE;
            if (controls > 4) controls = 0;
            if (controls == CONTROLS_JOYSTICK && !joysticks) controls = CONTROLS_KEYBOARD;
#ifdef USE_GLUT
            if (controls == CONTROLS_KEYBOARD) controls = CONTROLS_MOUSE;
            if (controls == CONTROLS_MOUSE_EXP) controls = CONTROLS_MOUSE;
#endif
          }
        }
      }
#ifdef HAVE_SDL_MIXER
      else if (ry >= 0.40 && ry <= 0.45)
      {
        if (sound->audio)
        {
          menuitemselected = 14;
          if (state == MOUSE_DOWN)
          {
            if (button == MOUSE_BUTTON_LEFT)
            {
              sound->volumesound += 10;
              if (sound->volumesound > 100)
                sound->volumesound = 0;
              sound->setVolume ();
              setPlaneVolume ();
              sound->play (SOUND_CLICK1);
              menu_reshape ();
            }
            else
            {
              sound->volumesound -= 10;
              if (sound->volumesound < 0)
                sound->volumesound = 100;
              sound->setVolume ();
              setPlaneVolume ();
              sound->play (SOUND_CLICK1);
              menu_reshape ();
            }
          }
        }
      }
      else if (ry >= 0.46 && ry <= 0.51)
      {
        if (sound->audio)
        {
          menuitemselected = 15;
          if (state == MOUSE_DOWN)
          {
            if (button == MOUSE_BUTTON_LEFT)
            {
              sound->volumemusic += 10;
              if (sound->volumemusic > 100)
              {
                sound->volumemusic = 0;
                sound->haltMusic ();
              }
              sound->setVolumeMusic ();
              menu_reshape ();
            }
            else
            {
              sound->volumemusic -= 10;
              if (sound->volumemusic < 0)
                sound->volumemusic = 100;
              sound->setVolumeMusic ();
              menu_reshape ();
            }
            if (sound->volumemusic != 0 && !sound->musicplaying)
              playRandomMusic ();
          }
        }
      }
#endif
      else if (ry >= 0.52 && ry <= 0.57)
      {
        menuitemselected = 16;
        if (state == MOUSE_DOWN)
        {
          if (button == MOUSE_BUTTON_LEFT)
          {
            difficulty ++;
            if (difficulty > 2) difficulty = 0;
          }
          else
          {
            difficulty --;
            if (difficulty < 0) difficulty = 2;
          }
        }
      }
      else if (ry >= 0.58 && ry <= 0.63)
      {
        menuitemselected = 17;
        if (state == MOUSE_DOWN)
        {
          if (button == MOUSE_BUTTON_LEFT)
          {
            brightness += 10;
            if (brightness > 50) brightness = -50;
          }
          else
          {
            brightness -= 10;
            if (brightness < -50) brightness = 50;
          }
        }
      }
    }
  }

#ifdef HAVE_SDL_NET
  if (menuitem == 7)
  {
    if (rx >= 0.48 && rx <= 0.85)
    {
      if (ry >= 0.15 && ry <= 0.2)
      {
        menuitemselected = 10;
        if (state == MOUSE_DOWN)
        {
          switch_create ();
        }
      }
      else if (ry >= 0.22 && ry <= 0.27)
      {
        menuitemselected = 11;
        if (state == MOUSE_DOWN)
        {
          switch_join ();
        }
      }
    }
  }
#endif

  if (lastitemselected != menuitemselected)
  {
//    menutimer = 0;
  }

  if (lastitem != menuitem)
  {
    sound->play (SOUND_CLICK1);
  }

#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void stats_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  statsitemselected = 0;
  if (ry >= 0.7 && ry <= 0.8)
  {
    if (rx >= 0.38 && rx <= 0.62)
    {
      statsitemselected = 1;
      if (state == MOUSE_DOWN)
      {
        stats_key (27, 0, 0);
      }
    }
  }

#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}



/****************************************************************************
  DISPLAY EVENTS
****************************************************************************/

void drawMissionElement (float x, float y, float z, int thismissionid, int missionid, int selected, char *string)
{
  int menutimernorm = menutimer * 5 / timestep;
  if (menutimernorm != 0) menutimernorm %= 360;
  if (menutimernorm < 0) menutimernorm *= -1;
  CColor color2 (255, 255, (int) (255.0 * cosi [menutimernorm]), 255);
  Pilot *p = pilots->pilot [pilots->aktpilot];
  if (p->mission_state [missionid] == 1)
  {
    if (menuitemselected == selected)
      font1->drawTextScaled (x, y, z, string, &color2, -menutimer * 5);
    else
      font1->drawText (x, y, z, string);
  }
  else
  {
    font1->drawText (x, y, z, string, &colorgrey);
  }
  drawMedal (x - 0.8, y + 0.6, z, getMedal (p->mission_score [thismissionid]), 1.0, thismissionid);
}

void stats_display ()
{
  float xf = 1.4, yf = 1.4, zf = -2.5;
  int c1 = 100, c2 = 10, c3 = 180;
  glBegin (GL_QUADS);
  glColor4ub (c2, c2, c2, 255);
  glVertex3f (-xf, -yf, zf);
  glColor4ub (c1, c1, c1, 255);
  glVertex3f (xf, -yf, zf);
  glColor4ub (c2, c2, c2, 255);

  glVertex3f (xf, yf, zf);
  glColor4ub (c1, c1, c1, 255);
  glVertex3f (-xf, yf, zf);
  glEnd ();
  glBegin (GL_LINE_STRIP);
  glColor4ub (c3, c3, c3, 255);
  glVertex3f (-xf, -yf, zf);
  glVertex3f (xf, -yf, zf);
  glVertex3f (xf, yf, zf);
  glVertex3f (-xf, yf, zf);
  glVertex3f (-xf, -yf, zf);
  glEnd ();

  drawMedal (-5.5, 6.5, -1.5, getMedal (fplayer->score), 1.6, mission->id);
  drawMedal (5.5, 6.5, -1.5, getMedal (fplayer->score), 1.6, mission->id);

  char buf [100];
  float xf1 = -12, xf2 = 0;
  yf = 6; zf = -2.5;
  CColor *color;
  Pilot *p = pilots->pilot [pilots->aktpilot];
  if (missionstate == 1)
  {
    font1->drawTextCentered (0, 6, -1.5, "SUCCESS", &colorblue);
    color = &colorblue;
  }
  else
  {
    font1->drawTextCentered (0, 6, -1.5, "FAILED", &colorred);
    color = &colorred;
  }
  font1->drawText (xf1, yf, zf, "SCORE:", color);
  sprintf (buf, "%d", fplayer->score);
  font1->drawText (xf2, yf, zf, buf, color);
  if (mission->difficulty != 1)
  {
    yf -= 1.5;
    font1->drawText (xf1, yf, zf, "DIFFICULTY:", color);
    if (mission->difficulty == 0) font1->drawText (xf2, yf, zf, "-50", color);
    else if (mission->difficulty == 2) font1->drawText (xf2, yf, zf, "+50", color);
  }
  int timebonus = 0;
  if (mission->timer < mission->maxtime)
    timebonus = (mission->maxtime - mission->timer) * 100 / mission->maxtime;
  yf -= 1.5;
  font1->drawText (xf1, yf, zf, "TIME BONUS:", color);
  sprintf (buf, "%d%%", timebonus);
  font1->drawText (xf2, yf, zf, buf, color);
  int shieldbonus = (int) (fplayer->shield * 100 / fplayer->maxshield);
  yf -= 1.5;
  font1->drawText (xf1, yf, zf, "SHIELD BONUS:", color);
  sprintf (buf, "%d%%", shieldbonus);
  font1->drawText (xf2, yf, zf, buf, color);
  yf -= 1.5;
  font1->drawText (xf1, yf, zf, "KILLS:", color);
  if (fplayer->fighterkills > 0)
  {
    sprintf (buf, "%d AIRCRAFTS", fplayer->fighterkills);
    font1->drawText (xf2, yf, zf, buf, color);
    yf -= 1.2;
  }
  if (fplayer->tankkills > 0)
  {
    sprintf (buf, "%d TANKS", fplayer->tankkills);
    font1->drawText (xf2, yf, zf, buf, color);
    yf -= 1.2;
  }
  if (fplayer->shipkills > 0)
  {
    sprintf (buf, "%d SHIPS", fplayer->shipkills);
    font1->drawText (xf2, yf, zf, buf, color);
    yf -= 1.2;
  }
  if (fplayer->otherkills > 0)
  {
    sprintf (buf, "%d OTHERS", fplayer->otherkills);
    font1->drawText (xf2, yf, zf, buf, color);
    yf -= 1.2;
  }
  if (ispromoted)
  {
    yf = -6;
    font1->drawTextCentered (0, yf, zf, "PROMOTED TO", color);
    yf -= 2;
    sprintf (buf, "%s", p->getRank ());
    font1->drawTextCentered (0, yf, zf, buf, color);
    drawRank (-11, yf + 0.5, zf, pilots->pilot [pilots->aktpilot]->ranking, 2);
    drawRank (7, yf + 0.5, zf, pilots->pilot [pilots->aktpilot]->ranking, 2);
  }
  yf = -9;
  if (!statsitemselected)
    font1->drawText (-3.5, yf, -2.0, "CONTINUE", &colorwhite);
  else
    font1->drawTextScaled (-3.5, yf, -2.0, "CONTINUE", &coloryellow, -menutimer * 5);
  drawMouseCursor ();
}

void menu_display ()
{
  char buf [100];
  int i;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  int menutimernorm = menutimer * 5 / timestep;
  if (menutimernorm != 0) menutimernorm %= 360;
  if (menutimernorm < 0) menutimernorm *= -1;
  CColor color2 (255, 255, (int) (255.0 * cosi [menutimernorm]), 255);

  int textx = -14, textx2 = 0;
  int normtimef = -menutimer * 5;

  if (menuitemselected == 0)
    font1->drawTextScaled (textx, 10, -2, "PILOTS", &color2, normtimef);
  else
    font1->drawText (textx, 10, -2, "PILOTS");

  if (menuitemselected == 1)
    font1->drawTextScaled (textx, 8, -2, "TRAINING", &color2, normtimef);
  else
    font1->drawText (textx, 8, -2, "TRAINING");

  if (menuitemselected == 2)
    font1->drawTextScaled (textx, 6, -2, "CAMPAIGN", &color2, normtimef);
  else
    font1->drawText (textx, 6, -2, "CAMPAIGN");

  if (menuitemselected == 3)
    font1->drawTextScaled (textx, 4, -2, "OPTIONS", &color2, normtimef);
  else
    font1->drawText (textx, 4, -2, "OPTIONS");

  if (menuitemselected == 4)
    font1->drawTextScaled (textx, 2, -2, "HELP", &color2, normtimef);
  else
    font1->drawText (textx, 2, -2, "HELP");

  if (menuitemselected == 5)
    font1->drawTextScaled (textx, 0, -2, "CREDITS", &color2, normtimef);
  else
    font1->drawText (textx, 0, -2, "CREDITS");

  if (menuitemselected == 6)
    font1->drawTextScaled (textx, -2, -2, "QUIT", &color2, normtimef);
  else
    font1->drawText (textx, -2, -2, "QUIT");

#ifdef HAVE_SDL_NET
  if (menuitemselected == 7)
    font1->drawTextScaled (textx, -4, -2, "MULTIPLAYER", &color2, normtimef);
  else
    font1->drawText (textx, -4, -2, "MULTIPLAYER");
#endif

  if (missionactive)
  {
    if (menuitemselected == 9)
      font1->drawTextScaled (textx, -6, -2, "RETURN", &color2, normtimef);
    else
      font1->drawText (textx, -6, -2, "RETURN");
  }

  if (menuitem == 0)
  {
    font1->drawText (textx2 - 2, 10, -2, "ACTIVE:", &coloryellow);
    if (pilots->aktpilot < pilots->aktpilots)
    {
      font1->drawText (textx2 + 4, 10, -2, pilots->pilot [pilots->aktpilot]->name, &coloryellow);
    }
    for (i = 0; i < pilots->aktpilots; i ++)
    {
      drawRank (textx2 - 1, 12.2 - (float) i * 3, -3, pilots->pilot [i]->ranking, 1.3);
      if (menuitemselected == 20 + i)
      {
        font1->drawTextScaled (textx2 + 3, 12.5 - (float) i * 3, -3, pilots->pilot [i]->getRank (), &color2, normtimef);
        font1->drawTextScaled (textx2 + 5, 11.5 - (float) i * 3, -3, pilots->pilot [i]->name, &color2, normtimef);
      }
      else
      {
        font1->drawText (textx2 + 3, 12.5 - (float) i * 3, -3, pilots->pilot [i]->getRank ());
        font1->drawText (textx2 + 5, 11.5 - (float) i * 3, -3, pilots->pilot [i]->name);
      }
    }
    if (menuitemselected == 10)
      font1->drawTextScaled (textx2 - 2, -5, -2.5, "DELETE (RIGHT MB)", &color2, normtimef);
    else
      font1->drawText (textx2 - 2, -5, -2.5, "DELETE (RIGHT MB)");
    if (menuitemselected == 11)
      font1->drawTextScaled (textx2 - 2, -7, -2.5, "CREATE", &color2, normtimef);
    else
      font1->drawText (textx2 - 2, -7, -2.5, "CREATE");
    pilotedit.draw (textx2, -9, -2.5, menutimer / 8);
  }
  else
    pilotedit.active = false;

  if (menuitem == 1)
  {
    Pilot *p = pilots->pilot [pilots->aktpilot];
    float zf = -3.0, yf = 18;
    if (menuitemselected == 10)
      font1->drawTextScaled (textx2, yf, zf, "TUTORIAL: PILOTING", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "TUTORIAL: PILOTING");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_TUTORIAL]), 1.0, MISSION_TUTORIAL);
    yf -= 1.5;
    if (menuitemselected == 11)
      font1->drawTextScaled (textx2, yf, zf, "TUTORIAL: DOGFIGHT", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "TUTORIAL: DOGFIGHT");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_DOGFIGHT]), 1.0, MISSION_DOGFIGHT);
    yf -= 2.0 * 1.5;
    if (menuitemselected == 13)
      font1->drawTextScaled (textx2, yf, zf, "FREE FLIGHT", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "FREE FLIGHT");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_FREEFLIGHT1]), 1.0, MISSION_FREEFLIGHT1);
    yf -= 1.5;
    if (menuitemselected == 14)
      font1->drawTextScaled (textx2, yf, zf, "DEATHMATCH", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "DEATHMATCH");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_DEATHMATCH1]), 1.0, MISSION_DEATHMATCH1);
    yf -= 1.5;
    if (menuitemselected == 15)
      font1->drawTextScaled (textx2, yf, zf, "TEAM DEATHMATCH", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "TEAM DEATHMATCH");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_DEATHMATCH2]), 1.0, MISSION_DEATHMATCH2);
    yf -= 1.5;
    if (menuitemselected == 16)
      font1->drawTextScaled (textx2, yf, zf, "TEAM BASE", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "TEAM BASE");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_TEAMBASE1]), 1.0, MISSION_TEAMBASE1);
    yf -= 1.5;
    if (menuitemselected == 17)
      font1->drawTextScaled (textx2, yf, zf, "WAVES", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "WAVES");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_WAVES1]), 1.0, MISSION_WAVES1);
    yf -= 1.5;
  }
  else if (menuitem == 2)
  {
    Pilot *p = pilots->pilot [pilots->aktpilot];
    float zf = -3.0, yf = 18;
    if (menuitemselected == 10)
      font1->drawTextScaled (textx2, yf, zf, "EAGLE TEST1", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "EAGLE TEST1");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_TEST1]), 1.0, MISSION_TEST1);
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_TEST2, MISSION_TEST1, 11, "EAGLE TEST2");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_TRANSPORT, MISSION_TEST2, 12, "TRANSPORT");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_CONVOY, MISSION_TRANSPORT, 13, "CONVOY");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_DOGFIGHT2, MISSION_CONVOY, 14, "DOGFIGHT");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_AIRBATTLE, MISSION_DOGFIGHT2, 15, "AIR BATTLE");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_SADEFENSE, MISSION_AIRBATTLE, 16, "SURFACE-AIR DEFENSE");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_SCOUT, MISSION_SADEFENSE, 17, "VETERAN DOGFIGHT");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_BASE, MISSION_SCOUT, 18, "BASE ATTACK");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_DEFEND1, MISSION_BASE, 20, "DEFEND SAM");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_DOGFIGHT3, MISSION_DEFEND1, 21, "DESERT DOGFIGHT");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_TANK1, MISSION_DOGFIGHT3, 22, "TANK ASSAUT");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_SHIP1, MISSION_TANK1, 25, "DESTROYERS");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_SHIP2, MISSION_SHIP1, 26, "OILRIG");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_SHIP3, MISSION_SHIP2, 27, "CRUISER");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_CANYON1, MISSION_SHIP3, 30, "RADAR BASE");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_CANYON2, MISSION_CANYON1, 31, "CANYON BATTLE");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_TUNNEL1, MISSION_CANYON2, 36, "TUNNEL");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_CANYON3, MISSION_TUNNEL1, 32, "MAIN BASE");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_MOON1, MISSION_CANYON3, 33, "TURRETS");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_MOON2, MISSION_MOON1, 34, "ELITE DOGFIGHT");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_MOON3, MISSION_MOON2, 35, "SNEAKING");
    zf = -2;
    if (menuitemselected == 100)
      font1->drawTextScaled (-2, -12, zf, "PILOTS", &color2, -menutimer * 5);
    else
      font1->drawText (-2, -12, zf, "PILOTS");
    zf = -2;
    if (menuitemselected == 101)
      font1->drawTextScaled (6, -12, zf, "FIGHTER", &color2, -menutimer * 5);
    else
      font1->drawText (6, -12, zf, "FIGHTER");
  }
  else if (menuitem == 3)
  {
    float yt = 12;
    sprintf (buf, "QUALITY: %d", quality);
    if (menuitemselected == 10)
      font1->drawTextScaled (textx2, yt -= 2, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yt -= 2, -2, buf);
    sprintf (buf, "VIEW: %d", (int) view);
    if (menuitemselected == 11)
      font1->drawTextScaled (textx2, yt -= 2, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yt -= 2, -2, buf);
    strcpy (buf, "DITHERING: ");
    if (dithering) strcat (buf, "ON");
    else strcat (buf, "OFF");
    if (menuitemselected == 12)
      font1->drawTextScaled (textx2, yt -= 2, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yt -= 2, -2, buf);
    strcpy (buf, "USE: ");
    if (controls == CONTROLS_KEYBOARD) strcat (buf, "KEYBOARD");
    else if (controls == CONTROLS_MOUSE) strcat (buf, "MOUSE EASY");
    else if (controls == CONTROLS_JOYSTICK) strcat (buf, "JOYSTICK");
    else if (controls == CONTROLS_MOUSE_REVERSE) strcat (buf, "MOUSE REVERSE");
    else if (controls == CONTROLS_MOUSE_EXP) strcat (buf, "MOUSE RELATIVE");
    if (menuitemselected == 13)
      font1->drawTextScaled (textx2, yt -= 2, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yt -= 2, -2, buf);
#ifdef HAVE_SDL_MIXER
    if (sound->audio)
      sprintf (buf, "SOUND: %d%%", (int) sound->volumesound);
    else
      sprintf (buf, "SOUND: N/A");
    if (menuitemselected == 14)
      font1->drawTextScaled (textx2, yt -= 2, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yt -= 2, -2, buf);
    if (sound->audio)
      sprintf (buf, "MUSIC: %d%%", (int) sound->volumemusic);
    else
      sprintf (buf, "MUSIC: N/A");
    if (menuitemselected == 15)
      font1->drawTextScaled (textx2, yt -= 2, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yt -= 2, -2, buf);
#else
    yt -= 4;
#endif
    strcpy (buf, "DIFFICULTY: ");
    if (difficulty == 0) strcat (buf, "EASY");
    else if (difficulty == 1) strcat (buf, "MEDIUM");
    else if (difficulty == 2) strcat (buf, "HARD");
    if (menuitemselected == 16)
      font1->drawTextScaled (textx2, yt -= 2, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yt -= 2, -2, buf);
    sprintf (buf, "BRIGHTNESS: %d %%", brightness);
    if (menuitemselected == 17)
      font1->drawTextScaled (textx2, yt -= 2, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yt -= 2, -2, buf);
  }
  else if (menuitem == 4)
  {
    int xs = -4, ys = 15;
    char buf [100];
    char text [100];
    font1->drawText (xs, ys --, -3, "KEYS:");
    sprintf (text, "%s\tFIRE CANNON", getKeyString (key_firecannon, buf));
    font1->drawText (xs, ys --, -3, text);
    sprintf (text, "%s\tFIRE MISSILE", getKeyString (key_firemissile, buf));
    font1->drawText (xs, ys --, -3, text);
    sprintf (text, "%s\tDROP CHAFF", getKeyString (key_dropchaff, buf));
    font1->drawText (xs, ys --, -3, text);
    sprintf (text, "%s\tDROP FLARE", getKeyString (key_dropflare, buf));
    font1->drawText (xs, ys --, -3, text);
    font1->drawText (xs, ys --, -3, "1..9\tCHANGE THROTTLE");
    sprintf (text, "%s\tINCREASE THROTTLE", getKeyString (key_thrustup, buf));
    font1->drawText (xs, ys --, -3, text);
    sprintf (text, "%s\tDECREASE THROTTLE", getKeyString (key_thrustdown, buf));
    font1->drawText (xs, ys --, -3, text);
    sprintf (text, "%s\tSELECT MISSILE", getKeyString (key_selectmissile, buf));
    font1->drawText (xs, ys --, -3, text);
    sprintf (text, "%s\tTARGET NEAREST ENEMY", getKeyString (key_targetnearest, buf));
    font1->drawText (xs, ys --, -3, text);
    sprintf (text, "%s\tTARGET LOCKING ENEMY", getKeyString (key_targetlocking, buf));
    font1->drawText (xs, ys --, -3, text);
    sprintf (text, "%s\tTARGET NEXT", getKeyString (key_targetnext, buf));
    font1->drawText (xs, ys --, -3, text);
    sprintf (text, "%s\tTARGET PREVIOUS", getKeyString (key_targetprevious, buf));
    font1->drawText (xs, ys --, -3, text);
    font1->drawText (xs, ys --, -3, "F1..8\tCAMERAS");
    font1->drawText (xs, ys --, -3, "ESC\tMAIN MENU");
    font1->drawText (xs, (-- ys) --, -3, "MOUSE:");
    font1->drawText (xs, ys --, -3, "SEE FILE \"CONF.INTERFACE\"");
    font1->drawText (xs, (-- ys) --, -3, "JOYSTICK:");
    font1->drawText (xs, ys --, -3, "SEE FILE \"CONF.INTERFACE\"");
    font1->drawTextCentered (0, -17, -3, "CONFIG FILES ARE LOCATED IN");
    strcpy (buf, dirs->getSaves (""));
    for (i = 0; i < strlen (buf); i ++)
      buf [i] = toupper (buf [i]);
    font1->drawTextCentered (0, -18.5, -3, buf);
  }
  else if (menuitem == 7)
  {
#ifdef HAVE_SDL_NET
    if (menuitemselected == 10)
      font1->drawTextScaled (textx2, 10, -2, "CREATE GAME", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, 10, -2, "CREATE GAME");
    if (menuitemselected == 11)
      font1->drawTextScaled (textx2, 8, -2, "JOIN GAME", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, 8, -2, "JOIN GAME");
#endif
  }

  drawMouseCursor ();
}

void pause_display ()
{
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  font1->drawText (-3, -1, -1, "PAUSED", &colorwhite);

  glPopMatrix ();
}

void credits_mouse (int button, int state, int x, int y)
{
  switch_menu ();
}

void credits_display ()
{
  float yt = 12, zf = -2.4;
  glTranslatef (0, -3.5 + 0.014 * (float) creditstimer / timestep, 0);
  CColor *col = &colorwhite;
  CColor *col2 = &coloryellow;
  font2->drawTextCentered (0, yt -= 2, zf, "GAME PROGRAMMING,", col);
  font2->drawTextCentered (0, yt -= 2, zf, "GRAPHICS, MODELS, SOUND & MUSIC", col);
  font1->drawTextCentered (0, yt -= 2, zf, "THOMAS A. DREXL", col2);
  font2->drawTextCentered (0, yt -= 4, zf, "LENS FLARES & FURTHER DEBUGGING", col);
  font1->drawTextCentered (0, yt -= 2, zf, "PIOTR PAWLOW", col2);
  font2->drawTextCentered (0, yt -= 4, zf, "MOUSE INTERFACE & LANDSCAPE IMPROVEMENTS", col);
  font1->drawTextCentered (0, yt -= 2, zf, "LOURENS VEEN", col2);
  font2->drawTextCentered (0, yt -= 4, zf, "PUBLISHING & FURTHER GAME IDEAS", col);
  font1->drawTextCentered (0, yt -= 2, zf, "BERNHARD KAINDL", col2);
  font2->drawTextCentered (0, yt -= 4, zf, "MOON TERRAIN", col);
  font1->drawTextCentered (0, yt -= 2, zf, "NORBERT DREXL", col2);
  font2->drawTextCentered (0, yt -= 4, zf, "IMPROVEMENTS TO PHYSICAL MODEL & COCKPIT", col);
  font1->drawTextCentered (0, yt -= 2, zf, "ARNE REINERS", col2);
}

void finish_display ()
{
  glTranslatef (0, -3.5 + 0.01 * (float) finishtimer / timestep, 0);
  CColor *col = &colorwhite;
  font1->drawTextCentered (0, 12, -3, "CONGRATULATIONS!", col);
  font1->drawTextCentered (0, 10, -3, "THE WORLD HAS BEEN SAVED YET AGAIN.", col);
  font1->drawTextCentered (0, 6, -3, "HOPE YOU HAD FUN PLAYING GL-117!", col);
  font1->drawTextCentered (0, 4, -3, "THIS GAME HAS ORIGINALLY BEEN DEVELOPED AS PART", col);
  font1->drawTextCentered (0, 2, -3, "OF THE COURSE \"APPLICATIONS OF COMPUTER GRAPHICS\"", col);
  font1->drawTextCentered (0, 0, -3, "AT THE TECHNICAL UNIVERSITY OF MUNICH, GERMANY.", col);
  font1->drawTextCentered (0, -2, -3, "IN FEBRUARY 2002 THE WORK WAS DONE AND I", col);
  font1->drawTextCentered (0, -4, -3, "PRESENTED THE PROTOTYPE OF A FLIGHT SIM,", col);
  font1->drawTextCentered (0, -6, -3, "YET WITHOUT TEXTURES, JOYSTICK, SOUNDS, MUSIC,", col);
  font1->drawTextCentered (0, -8, -3, "NO CAMPAIGN!", col);
  font1->drawTextCentered (0, -10, -3, "TWO MONTHS LATER THE FIRST RELEASE OF GL-117 WAS READY.", col);
  font1->drawTextCentered (0, -12, -3, "FURTHER RELEASES FOLLOWED, FEEDBACK ARRIVED, AND", col);
  font1->drawTextCentered (0, -14, -3, "PROGRAMMERS JOINED (LISTED IN THE CREDITS SECTION).", col);
  font1->drawTextCentered (0, -16, -3, "SPECIAL THANKS TO JOSEF DREXL FOR THE MODELING IDEAS,", col);
  font1->drawTextCentered (0, -18, -3, "AND THE UNIX AWARD GOES TO WOLFGANG HOMMEL ;-)", col);
}

void quit_display ()
{
  float xf = 1.0, yf = 0.5, zf = -2.5;
  int c1 = 100, c2 = 10, c4 = 180;
  glBegin (GL_QUADS);
  glColor4ub (c2, c2, c2, 255);
  glVertex3f (-xf, -yf, zf);
  glColor4ub (c1, c1, c1, 255);
  glVertex3f (xf, -yf, zf);
  glColor4ub (c2, c2, c2, 255);
  glVertex3f (xf, yf, zf);
  glColor4ub (c1, c1, c1, 255);
  glVertex3f (-xf, yf, zf);
  glEnd ();
  glBegin (GL_LINE_STRIP);
  glColor4ub (c4, c4, c4, 255);
  glVertex3f (-xf, -yf, zf);
  glVertex3f (xf, -yf, zf);
  glVertex3f (xf, yf, zf);
  glVertex3f (-xf, yf, zf);
  glVertex3f (-xf, -yf, zf);
  glEnd ();

  font1->drawTextCentered (0, 0.8, -1.5, "REALLY QUIT?", &colorblue);
  if (missionmenuitemselected == 0)
    font1->drawTextScaled (-4, -1.5, -1.8, "YES", &colorblue, -missionmenutimer * 5, 0);
  else
    font1->drawText (-4, -1.5, -1.8, "YES", &colorblue, 0);
  if (missionmenuitemselected == 1)
    font1->drawTextScaled (2, -1.5, -1.8, "NO", &colorblue, -missionmenutimer * 5, 0);
  else
    font1->drawText (2, -1.5, -1.8, "NO", &colorblue, 0);

  drawMouseCursor ();
}



/****************************************************************************
  IN-GAME DISPLAY EVENT
****************************************************************************/

int vibration = 0;
int starttime;

void game_display ()
{
  int i;
  double sunx, suny, sunz;

  if (dithering) glEnable (GL_DITHER);
  else glDisable (GL_DITHER);

  bool sunvisible = false;
  float pseudoview = getView ();

  float mycamtheta = camtheta, mycamphi = camphi, mycamgamma = camgamma;

  if (vibration > 0)
  {
    if (camera == 0)
    {
      float sinvib = sin ((float) vibration / timestep);
      mycamphi += 0.2 * vibration * sinvib / timestep;
      mycamgamma += 0.2 * vibration * sinvib / timestep;
    }
  }


  // calculate light factor
  if (camera == 0 && sunblinding && day && weather == WEATHER_SUNNY)
  {
    float np = fplayer->phi;
    if (np >= 180) np -= 360;
    float sunfactor = fabs (np) + fabs (fplayer->gamma - 180 - sungamma);
    if (sunfactor < 50)
    {
      sunfactor = (50 - sunfactor) / 10;
      if (sunfactor < 1.0) sunfactor = 1.0;
    }
    else sunfactor = 1.0;
    sunlight_dest = sunfactor;
    if (game == GAME_PLAY)
      pseudoview /= sunfactor;
  }
  else
  {
    if (day) sunlight_dest = 1.0;
    else sunlight_dest = 0.75;
    if (flash > 0)
    {
      sunlight_dest = (float) flash / timestep;
    }
  }

  // show a short flash when an object explodes
  if (quality > 0)
  {
    for (i = 0; i < maxfighter; i ++)
    {
      if (fighter [i]->draw)
        if (fighter [i]->explode > 0)
          if (fighter [i] != fplayer)
          {
            float dgamma = fabs (fplayer->getAngleH (fighter [i]));
            float dphi = fabs (fplayer->getAngle (fighter [i]));
            if (dphi < 45 && dgamma < 45)
            {
              float ddist = fplayer->distance (fighter [i]);
              if (ddist < 40)
              {
                ddist /= 15;
                ddist ++;
                dphi /= 25;
                dphi ++;
                dgamma /= 25;
                dgamma ++;
                if (fighter [i]->explode < 8 * timestep)
                  sunlight_dest = (float) fighter [i]->explode / timestep * 4 / ddist / dphi / dgamma;
                else if (fighter [i]->explode < 16 * timestep)
                  sunlight_dest = (16.0 - fighter [i]->explode / timestep) * 4 / ddist / dphi / dgamma;
              }
            }
          }
    }
  }

  // sunlight for glittering does not look good
/* if (camera == 0 && l->glittering > 1.0F)
  {
    sunlight += l->glittering * 1.0F - 1.0F;
  }*/
  l->glittering = 0;

  // turn down global sunlight when menu is showing
  if (game != GAME_PLAY && sunlight > 0.9F) sunlight = 0.9F;

  // start rendering

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glScalef (GLOBALSCALE, GLOBALSCALE, GLOBALSCALE);

  glShadeModel (GL_SMOOTH);

  glPushMatrix ();
  
  if (camera != 1 && camera != 5)
    glRotatef (-mycamtheta, 0.0, 0.0, 1.0);
  glRotatef (mycamgamma, 1.0, 0.0, 0.0);
  glRotatef (-mycamphi, 0.0, 1.0, 0.0);

  // draw sky

  glDisable (GL_FOG);
  glDisable (GL_DEPTH_TEST);

  float mylight = sunlight;
  if (!day) mylight /= 0.75;
  if (mylight > 1.0 && day)
    mylight = mylight / 5.0 + 0.8;
  else if (mylight > 1.0 && !day)
    mylight = mylight / 5.0 + 0.8;
  gl->foglum = mylight;
  sphere->drawGL (tlminf, tlinf, tlnull, space->alpha, mylight, true, false);

  if (quality >= 1 && clouds > 0)
  {
    gl->enableFog (pseudoview);

    if (quality >= 3)
    {
      highclouds2->zoom = 350;
      float ch2 = -332 - fplayer->tl->y / 10.5;
      CVector3 tlsphere3 (0, ch2, 0);
      highclouds2->drawGL (&tlsphere3, fplayer->tl);
    }

    highclouds->zoom = 300;
    float ch2 = -288 - fplayer->tl->y / 10.0;
    CVector3 tlsphere2 (0, ch2, 0);
    highclouds->drawGL (&tlsphere2, fplayer->tl);

    glDisable (GL_FOG);
  }

  if (weather == WEATHER_SUNNY || weather == WEATHER_CLOUDY)
  {
    if (!day)
    {
      glPointSize (1.0);
      int stars = maxstar;
      if (weather != WEATHER_CLOUDY) stars = maxstar / 2;
      for (i = 0; i < stars; i ++)
      {
        glPushMatrix ();
        glRotatef (star [i]->phi, 0.0, 1.0, 0.0);
        glRotatef (star [i]->gamma, 1.0, 0.0, 0.0);
        glTranslatef (0, 0, -10);
        star [i]->draw ();
        glPopMatrix ();
      }
    }
  }

  glEnable (GL_DEPTH_TEST);
  glEnable (GL_FOG);


// draw sun or moon (or earth)
  float fac = view, zfac = view * 0.2;
  if (weather == WEATHER_SUNNY || weather == WEATHER_CLOUDY)
  {
    if (camera == 0)
      glRotatef (sungamma, 1.0, 0.0, 0.0);
    else
      glRotatef (mycamgamma + sungamma, 1.0, 0.0, 0.0);
    float zf = -11;
    if (day)
      zf = -10;
    if (l->type == LAND_MOON && !day)
      zf = -8; // diplay bigger earth
    glTranslatef (0, 0, zf);
    gl->extractFrustum ();
    if (gl->isPointInFrustum (-1, 1, 0) || gl->isPointInFrustum (-1, -1, 0) ||
        gl->isPointInFrustum (1, -1, 0) || gl->isPointInFrustum (1, 1, 0))
    {
      glDisable (GL_DEPTH_TEST);
      if (day) gl->enableTextures (texsun->textureID);
      else if (l->type != LAND_MOON) gl->enableTextures (texmoon->textureID);
      else gl->enableTextures (texearth->textureID);
      if (day && l->type != 1)
        glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      gl->enableAlphaBlending ();
      glEnable (GL_ALPHA_TEST);
      float alphamax = 0.1;
      if (day) alphamax = 0.1;
      glAlphaFunc (GL_GEQUAL, alphamax);
      glBegin (GL_QUADS);
      fac = view; zfac = view * 3.5;
      if (day && l->type != LAND_MOON)
      {
        float gm = (40.0 - sungamma) / 80.0;
        if (gm < 0) gm = 0;
        if (gm > 0.5) gm = 0.5;
        glColor4f (1.0, 1.0 - gm, 0.8 - gm, 1.0);
      }
      else
        glColor4f (1.0, 1.0, 1.0, 1.0);
      glTexCoord2d (0, 1);
      glVertex3f (-1, 1, 0);
      glTexCoord2d (1, 1);
      glVertex3f (1, 1, 0);
      glTexCoord2d (1, 0);
      glVertex3f (1, -1, 0);
      glTexCoord2d (0, 0);
      glVertex3f (-1, -1, 0);
      glEnd ();

      // Where is the sun?
      double proj[16];
      double modl[16];
      int vp[4];
      glGetDoublev( GL_PROJECTION_MATRIX, proj );
      glGetDoublev( GL_MODELVIEW_MATRIX, modl );
      glGetIntegerv( GL_VIEWPORT, vp );
      gluProject (0, 0, 0, modl, proj, vp, &sunx, &suny, &sunz);
      if ((sunx>=vp[0])&&(suny>=vp[1])&&(sunx<(vp[0]+vp[2]))&&(suny<(vp[1]+vp[3]))) sunvisible = true;

      glDisable (GL_ALPHA_TEST);
      glEnable (GL_DEPTH_TEST);
      glDisable (GL_TEXTURE_2D);
      gl->disableAlphaBlending ();
    }
  }

  glPopMatrix ();
    
  glPushMatrix ();

  if (camera != 1 && camera != 5)
    glRotatef (-mycamtheta, 0.0, 0.0, 1.0);

  glRotatef (mycamgamma, 1.0, 0.0, 0.0);

  glDisable (GL_FOG);

  glRotatef (-mycamphi, 0.0, 1.0, 0.0);
  glScalef (1, 1, 1);

  glTranslatef (-camx, -camy, -camz);

  if (camera != 50)
  {
    gl->enableFog (pseudoview);
  }

  // draw terrain
  l->calcDynamicLight (explosion, laser, (DynamicObj **) missile, flare);
  glEnable (GL_CULL_FACE);
  l->draw ((int) mycamphi, (int) (-mycamgamma + 180.0));
  glDisable (GL_CULL_FACE);

  // draw objects
  gl->extractFrustum ();
  if (camera != 50)
  {
    space->lum = sunlight;
    for (i = 0; i < space->no; i ++)
    {
      if (space->o [i]->tl->y < l->getExactRayHeight (space->o [i]->tl->x, space->o [i]->tl->z))
        space->o [i]->lum = 0.5;
      else
        space->o [i]->lum = 1.0;
    }
    if (flash > 7 * timestep)
    {
      if (quality <= 2)
        flash1->draw ();
      else
        flash1->drawHQ ();
    }
    if (lighting)
    {
      glEnable( GL_LIGHTING);
      glEnable( GL_LIGHT0 );
      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      space->drawGL (); // draw all objects
      glDisable (GL_LIGHTING);
      glDepthMask (GL_FALSE);
      for (i = 0; i < space->no; i ++)
      {
        AIObj *dobj = (AIObj *) space->o [i];
        if (dobj->id >= MISSILE1)
          if (dobj->draw && dobj->drawlight && dobj->active)
          {
            if (dobj->smoke)
              if ((dobj->id >= MISSILE1 && dobj->id <= MISSILE2) || (dobj->id >= FIGHTER1 && dobj->id <= FIGHTER2))
                if (!(dobj->ttl == 0 && dobj->id >= MISSILE1 && dobj->id <= MISSILE2))
                {
                  dobj->smoke->draw ();
                }
          }
      }
      glDepthMask (GL_TRUE);
    }
    else
    {
      space->drawGL ();
    }
  }
  glDisable (GL_TEXTURE_2D);

  glDisable (GL_LIGHTING);

  if (camera == 0)
  {
    glDisable (GL_DEPTH_TEST);
    if (quality > 0)
      glEnable (GL_LINE_SMOOTH);
    cockpit->drawTargeter ();
    if (quality > 0)
      glDisable (GL_LINE_SMOOTH);
    glEnable (GL_DEPTH_TEST);
  }

  glPopMatrix ();

// draw flares
  if (quality > 0)
    if (sunvisible && /*camera == 0 &&*/ sunblinding && day)
    {
      CTexture* fl_texture[]= {texflare1,texflare3,texflare2,texflare4,texflare2,texflare4,texflare3,0};
      double fl_position[]=   {0.2,      1.6,      3.2,      8.1,      -1.4,     -2.2,     -3.5,     0};
      double fl_size[]=       {0.8,      0.6,      0.5,      0.4,      0.8,      0.4,      0.5,      0};
      double proj[16];
      double modl[16];
      double dr[2];
      int vp[4];
      double objx,objy,objz;

      glGetDoublev( GL_PROJECTION_MATRIX, proj );
      glGetDoublev( GL_MODELVIEW_MATRIX, modl );
      glGetIntegerv( GL_VIEWPORT, vp );
      glGetDoublev( GL_DEPTH_RANGE, dr );

      double cx=vp[2]/2+vp[0];
      double cy=vp[3]/2+vp[1];

      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      int i=0;
      double flarez_eye=4;
      double flarez_win=(-proj[10]+proj[14]/flarez_eye)*(dr[1]-dr[0])/2+(dr[1]+dr[0])/2;
      while (CTexture *tex=fl_texture[i])
      {
        double position=fl_position[i];
        double flarex,flarey,size=fl_size[i]/GLOBALSCALE;

        i++;

        flarex=(cx-sunx)*position+sunx;
        flarey=(cy-suny)*position+suny;

        if (gluUnProject (flarex, flarey, flarez_win, modl, proj, vp, &objx, &objy, &objz)==GL_TRUE)
        {
          gl->enableTextures (tex->textureID);
          gl->enableAlphaBlending ();
          glDisable (GL_ALPHA_TEST);
          glDisable (GL_DEPTH_TEST);
          glDisable (GL_FOG);

          glBegin (GL_QUADS);
          glColor4f (1.0, 1.0, 1.0, 1.0);
          glTexCoord2d (0, 1);
          glVertex3f (-size+objx, size+objy, objz);
          glTexCoord2d (1, 1);
          glVertex3f (size+objx, size+objy, objz);
          glTexCoord2d (1, 0);
          glVertex3f (size+objx, -size+objy, objz);
          glTexCoord2d (0, 0);
          glVertex3f (-size+objx, -size+objy, objz);
          glEnd ();
        }
      }
      glEnable (GL_DEPTH_TEST);
      glDisable (GL_TEXTURE_2D);
      gl->disableAlphaBlending ();
      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

// sunblinding test

  if (sunvisible)
  {
    GLfloat zbuf[1];
    glReadPixels((int)sunx,(int)suny,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,zbuf);
    sunblinding=(zbuf[0]<1)?false:true;
  }
  else
  {
    sunblinding=false;
  }

  // draw cockpit
  if (camera == 0)
  {
    if (quality > 0)
      glEnable (GL_LINE_SMOOTH);
    cockpit->drawCross ();
    cockpit->drawHeading ();
    if (quality > 0)
      glDisable (GL_LINE_SMOOTH);
    cockpit->drawRadar ();
    cockpit->drawTargetedElement ();
    cockpit->drawWeapon ();
    cockpit->drawCounter ();
  }

  // draw blackout/redout (blending)
  if (camera == 0)
  {
    int black = (int) blackout;
    if (black > 255) black = 255;
    int red = (int) redout;
    if (red > 255) red = 255;
    float xf = 2.0, yf = 1.5, zf = 1.0;
    if (black > 0)
    {
      glColor4ub (0, 0, 0, black);
    }
    else if (red > 0)
    {
      glColor4ub (255, 0, 0, red);
    }
    if (black > 0 || red > 0)
    {
      glDisable (GL_DEPTH_TEST);
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBegin (GL_QUADS);
      glVertex3f (-xf, -yf, -zf);
      glVertex3f (-xf, yf, -zf);
      glVertex3f (xf, yf, -zf);
      glVertex3f (xf, -yf, -zf);
      glEnd ();
      gl->disableAlphaBlending ();
      glDisable (GL_BLEND);
      glEnable (GL_DEPTH_TEST);
    }
  }

  if (brightness)
  {
    adjustBrightness ();
    glEnable (GL_DEPTH_TEST);
  }

  // draw mission dependant informations
  mission->draw ();

  glPushMatrix ();
  glDisable (GL_DEPTH_TEST);

  glPopMatrix ();
  char buf [25];
  sprintf (buf, "FPS: %d", (int) fps);
  font1->drawText (-25, 25, -3.5, buf, &colorwhite);

  bool write = false;
  if (firststart)
  {
    int akttime;
#ifndef USE_GLUT
    akttime = SDL_GetTicks ();
#else
    akttime = glutGet (GLUT_ELAPSED_TIME);
#endif
    if (akttime - starttime < 15000)
    {
      font1->drawTextCentered (0, -8, -2, "PLEASE WAIT WHILE", &colorred);
      font1->drawTextCentered (0, -9, -2, "ADJUSTING QUALITY", &colorred);
      write = true;
    }
  }
  if (fps <= 20 && !write)
  {
    font1->drawTextCentered (0, -8, -2, "FPS TOO LOW", &colorred);
    font1->drawTextCentered (0, -9, -2, "TURN DOWN VIEW OR QUALITY", &colorred);
  }

  if (controls == CONTROLS_MOUSE || controls == CONTROLS_MOUSE_REVERSE)
    drawMouseCursor ();
}



/****************************************************************************
  TIMER EVENTS
****************************************************************************/

float lastthrust;
int gametimer;

void game_timer (int dt)
{
//  if (multiplayer) return;
  int i, i2;

  sunlight += (sunlight_dest - sunlight) / 10 * dt / timestep;

  gametimer += dt;
  cockpit->dt = dt;
  l->lsticker += dt;
  if (l->lsticker >= 36000000)
    l->lsticker = 0;

  if (vibration > 0)
  {
    vibration -= dt;
  }

  if (fplayer->autofire && fplayer->active)
  {
    if (fplayer->ammo != 0)
    {
      fplayer->fireCannon (laser);
#ifdef HAVE_SDL_MIXER
      if (!startcannon)
      {
        sound->playLoop (SOUND_CANNON1);
        startcannon = true;
      }
#else
      sound->play (SOUND_CANNON1);
#endif
    }
  }
#ifdef HAVE_SDL_MIXER
  else
  {
    if (startcannon)
    {
      startcannon = false;
      sound->stop (SOUND_CANNON1);
    }
  }
#endif

  if (lastshield > fplayer->shield && !fplayer->ai)
  {
    sound->play (SOUND_HIT1);
    lastshield = (int) fplayer->shield;
    vibration = 25 * timestep;
  }

  // create flash during thunderstorm
  if (weather == WEATHER_THUNDERSTORM && flash <= 0 && !myrandom (2000 / dt))
  {
    flash = 18 * timestep;
    int fphi = (int) camphi + myrandom (50) - 25;
    if (fphi < 0) fphi += 360;
    else if (fphi >= 360) fphi -= 360;
    float pseudoview = getView ();
    float fdist = myrandom ((int) pseudoview - 20) + 10;
    float fx = fplayer->tl->x - sine [fphi] * fdist;
    float fz = fplayer->tl->z - cosi [fphi] * fdist;
    flash1->set (fx, l->getHeight (fx, fz), fz, (int) camphi);
    int lev = (int) (128.0 - 80.0 * fdist / (pseudoview - 10));
    sound->setVolume (SOUND_THUNDER1, lev);
    sound->play (SOUND_THUNDER1);
  }

  if (initing) return;

  if (flash > 0)
    flash -= dt;
  if (flash <= 7 * timestep && flash > 0)
    flash -= dt;

  if (lastthrust != fplayer->thrust && !((gametimer / timestep) & 15))
    setPlaneVolume ();
  lastthrust = fplayer->thrust;

  // collision tests
  for (i = 0; i < maxfighter; i ++)
  {
    for (i2 = 0; i2 < maxlaser; i2 ++)
      if (laser [i2]->active)
        fighter [i]->collide (laser [i2], dt);
    for (i2 = 0; i2 < maxmissile; i2 ++)
      if (missile [i2]->active)
        fighter [i]->collide (missile [i2], dt);
    for (i2 = 0; i2 < maxfighter; i2 ++)
      if (fighter [i2]->active)
        if (i != i2)
          fighter [i]->collide (fighter [i2], dt);
  }

  for (i = 0; i < maxflare; i ++)
  {
    for (i2 = 0; i2 < maxmissile; i2 ++)
      if (missile [i2]->active)
        flare [i]->collide (missile [i2], dt);
  }

  for (i = 0; i < maxchaff; i ++)
  {
    for (i2 = 0; i2 < maxmissile; i2 ++)
      if (missile [i2]->active)
        chaff [i]->collide (missile [i2], dt);
  }

  // move objects
  for (i = 0; i < maxfighter; i ++)
  {
    fighter [i]->aiAction (dt, (AIObj **) fighter, missile, laser, flare, chaff);
    float lev;
    if (fighter [i]->explode == 1 && (lev = fplayer->distance (fighter [i])) < 32)
    {
      sound->setVolume (SOUND_EXPLOSION1, 128 - (int) (lev * 4.0));
      sound->play (SOUND_EXPLOSION1);
    }
  }
  for (i = 0; i < maxlaser; i ++)
  {
    laser [i]->move (dt);
  }
  for (i = 0; i < maxmissile; i ++)
  {
    missile [i]->aiAction (dt, (AIObj **) fighter, missile, laser, flare, chaff);
  }
  for (i = 0; i < maxflare; i ++)
  {
    flare [i]->move (dt);
  }
  for (i = 0; i < maxchaff; i ++)
  {
    chaff [i]->move (dt);
  }

  for (i = 0; i < maxexplosion; i ++)
    explosion [i]->move (dt);
  for (i = 0; i < maxblacksmoke; i ++)
    blacksmoke [i]->move (dt);

  // show blackout/redout
  if (blackout > 0) blackout -= 3.0F * dt / timestep;
  if (blackout < 0) blackout = 0;
  if (redout > 0) redout -= 3.0F * dt / timestep;
  if (redout < 0) redout = 0;
  if (blackout > 400) blackout = 400;
  if (redout > 400) redout = 400;
  float testout;
  if (redout < 1)
  {
    testout = (fplayer->realspeed * fplayer->elevatoreffect - 0.13F) * 40.0F;
    if (testout > 0) blackout += testout * dt / timestep;
  }
  if (blackout < 1)
  {
    testout = (fplayer->realspeed * fplayer->elevatoreffect + 0.065F) * 80.0F;
    if (testout < 0) redout -= testout * dt / timestep;
  }

  // show own explosion from chase cam
  if (fplayer->shield <= 0)
    camera = 1;

  float cf = -fplayer->zoom / 2;
  camtheta = fplayer->theta;
  if (camera == 0)  // cockpit
  {
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + fplayer->zoom / 3.0;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi;
    camgamma = -fplayer->gamma + 180;
    fplayer->draw = 0;
  }
  if (camera == 1) // chase
  {
    cf = fplayer->zoom * 2;
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi;
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 2) // backwards
  {
    cf = -fplayer->zoom * 2;
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi + 180.0;
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 3) // other players
  {
    cf = fighter [aktcam]->zoom * 2;
    camx = fighter [aktcam]->tl->x + cf * SIN(fighter [aktcam]->phi);
    camy = fighter [aktcam]->tl->y + fighter [aktcam]->zoom;
    camz = fighter [aktcam]->tl->z + cf * COS(fighter [aktcam]->phi);
    camphi = fighter [aktcam]->phi;
    camgamma = 20;
    camtheta = fighter [aktcam]->theta;
    fplayer->draw = 1;
  }
  else if (camera == 4) // missile
  {
    cf = missile [0]->zoom * 10;
    camx = missile [0]->tl->x + cf * SIN(missile [0]->phi);
    camy = missile [0]->tl->y + fplayer->zoom * 2;
    camz = missile [0]->tl->z + cf * COS(missile [0]->phi);
    camphi = missile [0]->phi;
    fplayer->draw = 1;
  }
  else if (camera == 5) // top
  {
    cf = fplayer->zoom * 15;
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + 5.5;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi;
    fplayer->draw = 1;
    camgamma = 50;
  }
  else if (camera == 6) // left
  {
    cf = fplayer->zoom * 2;
    camphi = fplayer->phi + 90.0;
    if (camphi >= 360) camphi -= 360;
    else if (camphi < 0) camphi += 360;
    camx = fplayer->tl->x + cf * SIN(camphi);
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * COS(camphi);
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 7) // right
  {
    cf = fplayer->zoom * 2;
    camphi = fplayer->phi + 270.0;
    if (camphi >= 360) camphi -= 360;
    else if (camphi < 0) camphi += 360;
    camx = fplayer->tl->x + cf * SIN(camphi);
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * COS(camphi);
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 8) // top near
  {
    cf = fplayer->zoom * 5;
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + 2.5;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi;
    fplayer->draw = 1;
    camgamma = 50;
  }
  else if (camera == 9) // top very near
  {
    cf = fplayer->zoom * 2;
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + 1.0;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi;
    fplayer->draw = 1;
    camgamma = 50;
  }
  else if (camera == 50)
  {
    camx = 20;
    camz = 80;
    camy = 250;
    camphi = 20;
    camgamma = 75;
    game = GAME_PAUSE;
  }
  if (camphi >= 360.0) camphi -= 360.0;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif

  fplayer->rectheta -= dtheta;
  fplayer->recgamma += dgamma;

  missionstate = mission->processtimer (dt);

  if (missionstate == 2)
  {
    missionending ++;
    if (missionending >= 25)
    {
      missionending = 0;
      fplayer->score = mission->getScore (missionstate);
      if (!mission->id == MISSION_DEMO)
      {
        switch_stats ();
      }
      else
      {
        pleaseWait ();
        createMission (MISSION_DEMO);
        game_levelInit ();
      }
    }
  }

  if (missionstate == 1)
  {
    missionending ++;
    if (missionending >= 25)
    {
      missionending = 0;
      fplayer->score = mission->getScore (missionstate);
      if (mission->id != MISSION_DEMO)
      {
        if (mission->id != MISSION_MOON3)
          switch_stats ();
        else
          switch_finish ();
      }
      else
      {
        pleaseWait ();
        createMission (MISSION_DEMO);
        game_levelInit ();
      }
    }
  }
}

float lastfps = -1;
int newcamera = 0;

void menu_timer (Uint32 dt)
{
  menutimer += dt;
  int cycle = (menutimer / timestep) % 256;
  if (cycle == 0)
  {
    newcamera = 0;
  }
  if (cycle == 200 && !newcamera)
  {
    newcamera = 1;
    if (camera == 5)
    {
      camera = 1;
      if (missile [0]->active)
        camera = 4;
    }
    else if (camera == 1 || camera == 4) camera = 2;
    else if (camera == 2) camera = 5;
  }

  int akttime;
#ifndef USE_GLUT
  akttime = SDL_GetTicks ();
#else
  akttime = glutGet (GLUT_ELAPSED_TIME);
#endif
  if (firststart)
    if (akttime - starttime < 15000)
    {
      if (lastfps != fps)
      {
        lastfps = fps;
        if (fps > 40)
        {
          if (view < quality * 10 + 60) view += 10;
          else { quality ++; view = quality * 10 + 30; }
        }
        else if (fps < 30)
        {
          if (view > quality * 10 + 30) view -= 10;
          else { quality --; view = quality * 10 + 60; }
        }
        menu_reshape ();
      }
    }
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void stats_timer (Uint32 dt)
{
  menutimer += dt;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void mission_timer (Uint32 dt)
{
  missionmenutimer += dt;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void credits_timer (Uint32 dt)
{
  creditstimer += dt;
  if (creditstimer > 550 * timestep)
    creditstimer = 0;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void finish_timer (Uint32 dt)
{
  finishtimer += dt;
  if (finishtimer > 800 * timestep)
    finishtimer = 0;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void create_timer (Uint32 dt)
{
#ifndef USE_GLUT
//  server->createSocketSet ();
  server->getClient ();
  SDL_Delay (100);
  mission_timer (dt);
#endif
}

void join_timer (Uint32 dt)
{
#ifndef USE_GLUT
  char buf [STDSIZE];
  if (client->sock == NULL) client->getServer ("127.0.0.1", "client1");
  else 
  {
  SDL_Delay (100);
  mission_timer (dt);
  client->getMessage (buf);
  
  if (buf [0] == 's')
  {
    createMission (MISSION_MULTIPLAYER_DOGFIGHT);
    game_levelInit ();
    switch_game ();
    missionactive = true;
  }
  }
#endif
}



/****************************************************************************
  GAME DATA INITIALIZATION
****************************************************************************/

// calculate missile rack positions for a fighter
void setMissiles (CModel *model)
{
  int i;
  CVector3 tlmissile (0, 0.3, 0.3);
  for (i = 0; i < model->numObjects; i ++)
  {
    if (model->object [i]->numVertices == 4)
    {
      CObject *o = model->object [i];
      float sumx = 0, sumz = 0;
      float maxy = 2;
      int i2;
      for (i2 = 0; i2 < o->numVertices; i2 ++)
      {
        sumx += o->vertex [i2].vector.x;
        if (o->vertex [i2].vector.y < maxy)
          maxy = o->vertex [i2].vector.y;
        sumz += o->vertex [i2].vector.z;
      }
      tlmissile.x = sumx / 4.0F;
      tlmissile.y = maxy;
      tlmissile.z = sumz / 4.0F;

      tlmissile.y = maxy;
      model->addRefPoint (&tlmissile);
    }
  }
}

// initialize game data
void myInit ()
{
  int i, i2;

  // create textures (OpenGL)
  texsun = gl->genTextureTGA (dirs->getTextures ("sun2.tga"), 1, -1, 0, true);
  texmoon = gl->genTextureTGA (dirs->getTextures ("moon1.tga"), 1, 2, 0, true);
  texearth = gl->genTextureTGA (dirs->getTextures ("earth.tga"), 1, 0, 0, true);
  texflare1 = gl->genTextureTGA (dirs->getTextures ("flare1.tga"), 1, -1, 0, true);
  texflare2 = gl->genTextureTGA (dirs->getTextures ("flare2.tga"), 1, -1, 0, true);
  texflare3 = gl->genTextureTGA (dirs->getTextures ("flare3.tga"), 1, -1, 0, true);
  texflare4 = gl->genTextureTGA (dirs->getTextures ("flare4.tga"), 1, -1, 0, true);
  texgrass = gl->genTextureTGA (dirs->getTextures ("grass1.tga"), 0, 0, 1, false);
  texrocks = gl->genTextureTGA (dirs->getTextures ("rocks1.tga"), 0, 0, 1, false);
  texwater = gl->genTextureTGA (dirs->getTextures ("water1.tga"), 0, 0, 1, false);
  texsand = gl->genTextureTGA (dirs->getTextures ("sand1.tga"), 0, 0, 1, false);
  texredsand = gl->genTextureTGA (dirs->getTextures ("redsand1.tga"), 0, 0, 1, false);
  texredstone = gl->genTextureTGA (dirs->getTextures ("redstone2.tga"), 0, 0, 1, false);
  textree = gl->genTextureTGA (dirs->getTextures ("tree1.tga"), 0, -1, 1, true);
  textreeu = gl->genTextureTGA (dirs->getTextures ("treeu1.tga"), 0, -1, 1, true);
  textree2 = gl->genTextureTGA (dirs->getTextures ("tree2.tga"), 0, -1, 1, true);
  textreeu2 = gl->genTextureTGA (dirs->getTextures ("treeu2.tga"), 0, -1, 1, true);
  textree3 = gl->genTextureTGA (dirs->getTextures ("tree3.tga"), 0, 3, 1, true);
  textreeu3 = gl->genTextureTGA (dirs->getTextures ("treeu3.tga"), 0, 3, 1, true);
  textree4 = gl->genTextureTGA (dirs->getTextures ("tree4.tga"), 0, 3, 1, true);
  textreeu4 = gl->genTextureTGA (dirs->getTextures ("treeu4.tga"), 0, 3, 1, true);
  texcactus1 = gl->genTextureTGA (dirs->getTextures ("cactus1.tga"), 0, 3, 1, true);
  texcactusu1 = gl->genTextureTGA (dirs->getTextures ("cactusu1.tga"), 0, 3, 1, true);
  texsmoke = gl->genTextureTGA (dirs->getTextures ("smoke1.tga"), 0, -1, 1, true);
  texsmoke2 = gl->genTextureTGA (dirs->getTextures ("smoke2.tga"), 0, -1, 1, true);
  texsmoke3 = gl->genTextureTGA (dirs->getTextures ("smoke3.tga"), 0, 5, 1, true);
  texcross = gl->genTextureTGA (dirs->getTextures ("cross.tga"), 0, -1, 1, true);
  texcross2 = gl->genTextureTGA (dirs->getTextures ("cross2.tga"), 0, -1, 1, true);
  texranks = gl->genTextureTGA (dirs->getTextures ("ranks.tga"), 0, 0, 0, true);
  texmedals = gl->genTextureTGA (dirs->getTextures ("medals.tga"), 0, 0, 0, true);
  texclouds1 = gl->genTextureTGA (dirs->getTextures ("clouds1.tga"), 0, 4, 1, true);
  texclouds2 = gl->genTextureTGA (dirs->getTextures ("clouds2.tga"), 0, 4, 1, true);
  texclouds3 = gl->genTextureTGA (dirs->getTextures ("clouds3.tga"), 0, 6, 1, true);
  texradar1 = gl->genTextureTGA (dirs->getTextures ("radar2.tga"), 0, -1, 0, true);
  texradar2 = gl->genTextureTGA (dirs->getTextures ("radar1.tga"), 0, -1, 0, true);
  texgravel1 = gl->genTextureTGA (dirs->getTextures ("gravel1.tga"), 0, 0, 1, false);
  texglitter1 = gl->genTextureTGA (dirs->getTextures ("glitter.tga"), 0, -1, 0, true);
  texarrow = gl->genTextureTGA (dirs->getTextures ("arrow.tga"), 0, -1, 0, true);

  // useful global variables/constants
  tlinf = new CVector3 (1E10, 1E10, 1E10);
  tlminf = new CVector3 (-1E10, -1E10, -1E10);
  tlnull = new CVector3 (0, 0, 0);
  rotnull = new CRotation ();
  rotmissile = new CRotation ();
  rotmissile->a = 90;
  rotmissile->c = 270;

  // initialize all global variables

  for (i = 0; i < maxgroundobj; i ++)
  {
    groundobj [i] = new DynamicObj (space, &model_tent1, 3);
  }

  explsphere = new CSphere ();
  ((CSphere *) explsphere)->init (1, 9);
  CColor explcolor (255, 255, 1);
  explsphere->setColor (&explcolor);
  explsphere->alpha = true;
  for (i = 0; i < explsphere->object [0]->numVertices; i ++)
  {
    explsphere->object [0]->vertex [i].color.setColor (myrandom (100) + 155, myrandom (100) + 100, 0, myrandom (3) / 2 * 255);
  }
  for (i = 0; i < maxexplosion; i ++)
  {
    explosion [i] = new CExplosion (space, explsphere);
  }

  for (i = 0; i < maxblacksmoke; i ++)
  {
    blacksmoke [i] = new CBlackSmoke (space);
  }

  for (i = 0; i < maxfighter; i ++)
  {
    fighter [i] = new AIObj (space, &model_fig, 0.4);
    for (i2 = 0; i2 < 12; i2 ++)
      fighter [i]->addRefModel (&model_missile1, tlnull, rotmissile, 0.2);
  }

  highclouds = new HighClouds (25);
  highclouds->setTexture (texclouds3);

  highclouds2 = new HighClouds (25);
  highclouds2->setTexture (texclouds3);

  objsphere = new CSphere (1, 9, 1, 1, 1);
  sphere = new CSpaceObj (objsphere, 10.0);
  sphere->rot->b = 90;
  sphere->draw = 2;
  sphere->drawlight = false;

  flash1 = new Flash ();

  for (i = 0; i < maxlaser; i ++)
  {
    laser [i] = new DynamicObj (space, &model_cannon1, 0.07);
  }

  for (i = 0; i < maxmissile; i ++)
  {
    missile [i] = new AIObj (space, &model_missile1, 0.1);
  }

  for (i = 0; i < maxflare; i ++)
  {
    flare [i] = new DynamicObj (space, &model_flare1, 0.1);
  }

  for (i = 0; i < maxchaff; i ++)
  {
    chaff [i] = new DynamicObj (space, &model_chaff1, 0.1);
  }

  for (i = 0; i < maxstar; i ++)
  {
    star [i] = new Star (myrandom (360), myrandom (85), 0.4 + 0.1 * myrandom (8));
  }

  cockpit = new Cockpit ();

  pleaseWait ();
  createMission (MISSION_DEMO);
  game_levelInit ();

  menu_reshape ();
}



/****************************************************************************
  INTRO
****************************************************************************/

CRotation rot;
CRotation rot2;
CVector3 tl;
CVector3 tl2;
int initexplode = 0;
int initexplode1 = 0;
int i;
int inittimer = 0;
int inittimer_gl117 = 0;

void init_reshape ()
{
  // use whole window
  glViewport (0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  
  // angle, aspectratio, nearclip, farclip
  gluPerspective (80.0, 1.0, nearclippingplane, 80);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
}

// load game data (this method does not really belong to the intro itself)
void myFirstInit ()
{
  display ("Creating calculation tables", LOG_ALL);
  mathtab_init ();

  display ("Creating advanced OpenGL methods", LOG_ALL);
  gl = new GL ();

  display ("Loading Fonts", LOG_ALL);
  font1 = new Font (dirs->getTextures ("font1.tga"), 32, '!', 64);
  font2 = new Font (dirs->getTextures ("font2.tga"), 32, '!', 64);

  display ("Loading 3ds models", LOG_ALL);
//  g_Load3ds.Import3DS (&model_fig, dirs->getModels ("fig1.3ds"));
  g_Load3ds.Import3DS (&model_fig, dirs->getModels ("gl-16.3ds"));
  model_fig.setName ("FALCON");
  model_fig.scaleTexture (0.3, 0.3);
//  g_Load3ds.Import3DS (&model_figa, dirs->getModels ("fig4.3ds"));
  g_Load3ds.Import3DS (&model_figa, dirs->getModels ("gl-15.3ds"));
  model_figa.setName ("SWALLOW");
  model_figa.scaleTexture (0.3, 0.3);
//  g_Load3ds.Import3DS (&model_figb, dirs->getModels ("fig2.3ds"));
  g_Load3ds.Import3DS (&model_figb, dirs->getModels ("gl-14c.3ds"));
  model_figb.setName ("HAWK");
  model_figb.scaleTexture (0.2, 0.2);
//  g_Load3ds.Import3DS (&model_figc, dirs->getModels ("fig3.3ds"));
  g_Load3ds.Import3DS (&model_figc, dirs->getModels ("gl-14d.3ds"));
  model_figc.setName ("HAWK II");
  model_figc.scaleTexture (0.2, 0.2);
//  g_Load3ds.Import3DS (&model_figd, dirs->getModels ("fig1.3ds"));
  g_Load3ds.Import3DS (&model_figd, dirs->getModels ("gl-21b.3ds"));
  model_figd.setName ("BUZZARD");
  model_figd.scaleTexture (0.3, 0.3);
//  g_Load3ds.Import3DS (&model_fige, dirs->getModels ("fig6.3ds"));
  g_Load3ds.Import3DS (&model_fige, dirs->getModels ("gl-21.3ds"));
  model_fige.setName ("CROW");
  model_fige.scaleTexture (0.3, 0.3);
//  g_Load3ds.Import3DS (&model_figf, dirs->getModels ("fig8.3ds"));
  g_Load3ds.Import3DS (&model_figf, dirs->getModels ("gl-14b.3ds"));
  model_figf.setName ("PHOENIX");
  model_figf.scaleTexture (0.2, 0.2);
//  g_Load3ds.Import3DS (&model_figg, dirs->getModels ("fig9.3ds"));
  g_Load3ds.Import3DS (&model_figg, dirs->getModels ("gl-14.3ds"));
  model_figg.setName ("RED ARROW");
  model_figg.scaleTexture (0.2, 0.2);
//  g_Load3ds.Import3DS (&model_figh, dirs->getModels ("fig5.3ds"));
  g_Load3ds.Import3DS (&model_figh, dirs->getModels ("gl-29.3ds"));
  model_figh.setName ("BLACKBIRD");
  model_figh.scaleTexture (0.3, 0.3);
  g_Load3ds.Import3DS (&model_figt, dirs->getModels ("transp1.3ds"));
  model_figt.setName ("TRANSPORT");

  // cannon at daylight
  g_Load3ds.Import3DS (&model_cannon1, dirs->getModels ("cannon1.3ds"));

  // cannon at night
  g_Load3ds.Import3DS (&model_cannon2, dirs->getModels ("cannon2.3ds"));
  model_cannon2.nolight = true;
  model_cannon2.alpha = true;
  for (i = 0; i < 4; i ++)
  {
    model_cannon2.object [0]->vertex [i].color.c [0] = 255;
    model_cannon2.object [0]->vertex [i].color.c [1] = 255;
    model_cannon2.object [0]->vertex [i].color.c [2] = 0;
    model_cannon2.object [0]->vertex [i].color.c [3] = 255;
  }
  model_cannon2.object [0]->vertex [1].color.c [3] = 50;
  model_cannon2.object [0]->vertex [2].color.c [3] = 50;

  g_Load3ds.Import3DS (&model_flare1, dirs->getModels ("flare1.3ds"));
  model_flare1.setName ("FLARE");
  model_flare1.alpha = true;
  model_flare1.nolight = true;
  g_Load3ds.Import3DS (&model_chaff1, dirs->getModels ("chaff1.3ds"));
  model_chaff1.setName ("CHAFF");
  model_chaff1.alpha = true;
  model_chaff1.nolight = true;
  g_Load3ds.Import3DS (&model_missile1, dirs->getModels ("missile1.3ds"));
  model_missile1.setName ("AAM HS MK1");
  g_Load3ds.Import3DS (&model_missile2, dirs->getModels ("missile2.3ds"));
  model_missile2.setName ("AAM HS MK2");
  g_Load3ds.Import3DS (&model_missile3, dirs->getModels ("missile3.3ds"));
  model_missile3.setName ("AAM HS MK3");
  g_Load3ds.Import3DS (&model_missile4, dirs->getModels ("missile4.3ds"));
  model_missile4.setName ("AGM MK1");
  g_Load3ds.Import3DS (&model_missile5, dirs->getModels ("missile5.3ds"));
  model_missile5.setName ("AGM MK2");
  g_Load3ds.Import3DS (&model_missile6, dirs->getModels ("missile6.3ds"));
  model_missile6.setName ("DFM");
  g_Load3ds.Import3DS (&model_missile7, dirs->getModels ("missile7.3ds"));
  model_missile7.setName ("AAM FF MK1");
  g_Load3ds.Import3DS (&model_missile8, dirs->getModels ("missile8.3ds"));
  model_missile8.setName ("AAM FF MK2");
  g_Load3ds.Import3DS (&model_flak1, dirs->getModels ("flak2.3ds"));
  model_flak1.setName ("SA CANNON");
  g_Load3ds.Import3DS (&model_flarak1, dirs->getModels ("flarak1.3ds"));
  model_flarak1.setName ("SAM");
  g_Load3ds.Import3DS (&model_ship1, dirs->getModels ("ship1.3ds"));
  model_ship1.setName ("CRUISER");
  g_Load3ds.Import3DS (&model_tent1, dirs->getModels ("tent1.3ds"));
  model_tent1.setName ("TENT");
  g_Load3ds.Import3DS (&model_gl117, dirs->getModels ("gl-117.3ds"));
  model_gl117.displaylist = false;
//  model_gl117.scaleTexture (0.10, 0.10);
  g_Load3ds.Import3DS (&model_tank1, dirs->getModels ("tank1.3ds"));
  model_tank1.setName ("WIESEL");
  model_tank1.scaleTexture (0.5, 0.5);
  g_Load3ds.Import3DS (&model_container1, dirs->getModels ("container1.3ds"));
  model_container1.setName ("CONTAINER");
  g_Load3ds.Import3DS (&model_ship2, dirs->getModels ("ship2.3ds"));
  model_ship2.setName ("LIGHT DESTROYER");
  g_Load3ds.Import3DS (&model_truck1, dirs->getModels ("truck1.3ds"));
  model_truck1.setName ("TRUCK");
  g_Load3ds.Import3DS (&model_pickup1, dirs->getModels ("pickup1.3ds"));
  model_pickup1.setName ("PICKUP");
  g_Load3ds.Import3DS (&model_pickup2, dirs->getModels ("pickup2.3ds"));
  model_pickup2.setName ("PICKUP");
  g_Load3ds.Import3DS (&model_tank2, dirs->getModels ("tank2.3ds"));
  model_tank2.setName ("PANTHER");
  model_tank2.scaleTexture (0.5, 0.5);
  g_Load3ds.Import3DS (&model_tent4, dirs->getModels ("tent4.3ds"));
  model_tent4.setName ("BIG TENT");
  g_Load3ds.Import3DS (&model_hall1, dirs->getModels ("hall1.3ds"));
  model_hall1.setName ("HALL");
  g_Load3ds.Import3DS (&model_hall2, dirs->getModels ("hall2.3ds"));
  model_hall2.setName ("HALL");
  g_Load3ds.Import3DS (&model_oilrig, dirs->getModels ("oilrig.3ds"));
  model_oilrig.setName ("OILRIG");
  model_oilrig.alpha = true;
  g_Load3ds.Import3DS (&model_egg, dirs->getModels ("egg.3ds"));
  model_egg.scaleTexture (8, 8);
  model_egg.setName ("COMPLEX");
  g_Load3ds.Import3DS (&model_radar, dirs->getModels ("radar.3ds"));
  model_radar.setName ("RADAR");
  g_Load3ds.Import3DS (&model_mine1, dirs->getModels ("mine1.3ds"));
  model_radar.setName ("MINE");
  g_Load3ds.Import3DS (&model_aster1, dirs->getModels ("aster1.3ds"));
  model_radar.setName ("ASTEROID");
  g_Load3ds.Import3DS (&model_base1, dirs->getModels ("base1.3ds"));
  model_radar.setName ("MOON BASE");
  g_Load3ds.Import3DS (&model_barrier1, dirs->getModels ("barrier.3ds"));
  model_barrier1.scaleTexture (10, 10);
  model_barrier1.alpha = true;

  setMissiles (&model_fig);
  setMissiles (&model_figa);
  setMissiles (&model_figb);
  setMissiles (&model_figc);
  setMissiles (&model_figd);
  setMissiles (&model_fige);
  setMissiles (&model_figf);
  setMissiles (&model_figg);
  setMissiles (&model_figh);

  // enable Z-Buffer
  glEnable (GL_DEPTH_TEST);

  // fill polygons (GL_LINE for wireframe models)
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

  display ("Setting up world geometry", LOG_ALL);
  space = new Space ();
  space->drawlight = true;
  clip1 = space->z1;
  clip2 = space->z2;
  clip1->x = -ZOOM;
  clip1->y = -ZOOM;
  clip1->z = -ZOOM;
  clip2->x = ZOOM;
  clip2->y = ZOOM;
  clip2->z = ZOOM;

  // prepare intro
  init_reshape ();

  tl.x = 0.0;
  tl.y = 0.0;
  tl.z = -5.0;
  tl2.x = 0.0;
  tl2.y = 0.0;
  tl2.z = -5.0;
  rot.a = 270;
  rot2.c = 90;
  rot2.a = 270;
  rot2.b = 270;
  initexplode = 0;
  initexplode1 = 0;

  sungamma = 60;
  setLightSource (60);

  glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  glEnable (GL_LIGHT0);
  glEnable (GL_LIGHTING);
  glEnable (GL_COLOR_MATERIAL);

  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
}

void init_key (int key, int x, int y)
{
  gl->clearScreen (); // exit intro
  myInit ();
  switch_menu ();
  fplayer->ai = true;
  camera = 5;
#ifndef USE_GLUT
  starttime = SDL_GetTicks ();
#else
  starttime = glutGet (GLUT_ELAPSED_TIME);
#endif
}

void init_mouse (int button, int state, int x, int y)
{
  init_key (32, x, y);
}

const int maxfx = 100;
const int maxfy = 30;

int heat [maxfy] [maxfx];
int heat2 [maxfy] [maxfx];

void init_display ()
{
  int i, i2;
  CVector3 vec;
  CColor color (200, 200, 200, 255);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glPushMatrix ();

  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);

  // draw fighter
  glPushMatrix ();
  glTranslatef (0, 0, -5);
  model_fig.draw (&vec, &tl, &rot, 1.0, 1.0, initexplode1);
  glPopMatrix ();

  // draw gl-117 logo
  if (initexplode1 < 0)
  {
    gl117_rotateColors (inittimer_gl117);
    glPushMatrix ();
    glTranslatef (0, 0, -5);
    model_gl117.draw2 (&vec, &tl2, &rot2, 1.0, initexplode);
    glPopMatrix ();
  }

  glDisable (GL_LIGHTING);
  glPopMatrix ();

  glPopMatrix ();

  // draw fire (heat array)
  glDisable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);
  float xf = 2.0F, yf = 2.0F, zf = 2.0F;
  glPushMatrix ();
  for (i = 0; i < maxfy; i ++)
  {
    glBegin (GL_QUAD_STRIP);
    for (i2 = 0; i2 < maxfx + 1; i2 ++)
    {
      // rotate through fire colors (white-yellow-red-black-blue-black)
      // col in [0...512]
      int yind = i;
      int r = heat [yind] [i2]; // blend out late for red->black
      if (r > 255) r = 255;
      int g = heat [yind] [i2] - 255; // blend out for yellow->red
      if (g > 255) g = 255;
      else if (g < 0) g = 0;
      int b = heat [yind] [i2] - 512; // blend out early to get white->yellow
      if (b > 255) b = 255;
      else if (b < -462) b = (512 + b) * 3; // insert blue shimmer very late
      else if (b < -412) b = (-412 - b) * 3;
      else if (b < 0) b = 0;
      int a = r >= b ? r : b; // alpha value: transparent after yellow-red phase
      glColor4ub (r, g, b, a);
      glVertex3f (-xf + 2.0F * xf * i2 / maxfx, yf - 2.0F * yf * yind / maxfy, -zf);

      // do the same for the next vertex
      yind = i + 1;
      r = heat [yind] [i2];
      if (r > 255) r = 255;
      g = heat [yind] [i2] - 255;
      if (g > 255) g = 255;
      else if (g < 0) g = 0;
      b = heat [yind] [i2] - 512;
      if (b > 255) b = 255;
      else if (b < -462) b = (512 + b) * 3;
      else if (b < -412) b = (-412 - b) * 3;
      else if (b < 0) b = 0;
      a = r >= b ? r : b;
      glColor4ub (r, g, b, a);
      glVertex3f (-xf + 2.0F * xf * i2 / maxfx, yf - 2.0F * yf * yind / maxfy, -zf);
    }
    glEnd ();
  }
  glPopMatrix ();
  glDisable (GL_BLEND);

  font2->drawText (-20, -20, -3, VERSIONSTRING, &color);
}

void genFireLine ()
{
  int i, i2;
  for (i = 0; i < maxfx; i ++)
  {
    heat [maxfy - 1] [i] = myrandom (255 * 2);
  }
  for (i = 0; i < 5; i ++)
  {
    int r = myrandom (maxfx + 1);
    for (i2 = 0; i2 <= 0; i2 ++)
    {
      heat [maxfy - 1] [r + i2] = 255 * 3 + 100; // insert hot spots at the bottom line
    }
  }
}

void proceedFire ()
{
  int i, i2;
  for (i = maxfy - 2; i >= 0; i --)
    for (i2 = 2; i2 < maxfx - 2; i2 ++)
    {
      heat2 [i] [i2] = (heat [i + 1] [i2 - 2] + 3 * heat [i + 1] [i2 - 1] + 8 * heat [i + 1] [i2] + 3 * heat [i + 1] [i2 + 1] + heat [i + 1] [i2 + 2]) / 16; // heat diffusion
      heat2 [i] [i2] -= (int) (500.0F / maxfy); // heat sink
      if (heat2 [i] [i2] < 0) heat2 [i] [i2] = 0;
    }
  memcpy (heat, heat2, maxfx * maxfy * sizeof (int)); // copy back buffer to heat array
}

int initsynchrotimer = 0;

void init_timer (Uint32 dt)
{
  inittimer_gl117 += dt;
  initsynchrotimer += dt;
  if (initsynchrotimer > 30)
  {
    initsynchrotimer -= 30;
    dt = 30;
  }
  else return;

  int r = myrandom (100);
  if (r == 50) r = myrandom (100); // do not optimize this: random number generator initialization

  tl.x = 6.0 * pow (1.5, -(5 + tl.z)) - 0.4;
  tl.y = 0.9 * tl.x;
  tl.z += 0.15;

  if (initexplode1 == -1 && tl2.z < 3)
  {
    tl2.z += 0.25;
  }
  if (tl2.z < 3)
  {
    rot2.b -= 20;
    if (rot2.b < 0) rot2.b += 360;
  }
  if (tl.z > -4.0)
  {
    rot.b ++;
  }

  if (inittimer == 0)
  {
    memset (heat, 0, maxfx * maxfy * sizeof (int));
    memset (heat2, 0, maxfx * maxfy * sizeof (int));
  }

  genFireLine ();
  proceedFire ();

  if (inittimer >= 75)
  {
    initexplode1 = -1;
  }

  if (inittimer >= 150)
  {
    initexplode += dt;
  }

  if (inittimer >= 200)
  {
    init_key (27, 0, 0); // switch to menu
  }

  inittimer ++;

#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}



/****************************************************************************
  ABSTRACT EVENT BRANCHES
****************************************************************************/

static void myKeyboardFunc (unsigned char key, int x, int y)
{
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_key (key, x, y);
  else if (game == GAME_INIT)
    init_key (key, x, y);
  else if (game == GAME_MENU)
    menu_key (key, x, y);
  else if (game == GAME_CREDITS)
    credits_key (key, x, y);
  else if (game == GAME_CREATE)
    create_key (key, x, y);
  else if (game == GAME_JOIN)
    join_key (key, x, y);
  else if (game == GAME_MISSION)
    mission_key (key, x, y);
  else if (game == GAME_STATS)
    stats_key (key, x, y);
  else if (game == GAME_FAME)
    fame_key (key, x, y);
  else if (game == GAME_FIGHTER)
    fighter_key (key, x, y);
  else if (game == GAME_FINISH)
    finish_key (key, x, y);
  else if (game == GAME_QUIT)
    quit_key (key, x, y);
}

static void myKeyboardFuncUp (unsigned char key, int x, int y)
{
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_keyup (key, x, y);
}

static void mySpecialFunc (int key, int x, int y)
{
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_keyspecial (key, x, y);
}

static void mySpecialFuncUp (int key, int x, int y)
{
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_keyspecialup (key, x, y);
}

static void myPassiveRelMotionFunc (int xrel, int yrel)
{
  if (game == GAME_PLAY)
    game_mouserelmotion (xrel, yrel);
}

static void myPassiveMotionFunc (int x, int y)
{
  mousex = x; mousey = y;
  if (game == GAME_PLAY)
    game_mousemotion (x, y);
  else if (game == GAME_MENU)
    menu_mouse (-1, -1, x, y);
  else if (game == GAME_STATS)
    stats_mouse (-1, -1, x, y);
  else if (game == GAME_MISSION)
    mission_mouse (-1, -1, x, y);
  else if (game == GAME_FAME)
    fame_mouse (-1, -1, x, y);
  else if (game == GAME_FIGHTER)
    fighter_mouse (-1, -1, x, y);
  else if (game == GAME_CREATE)
    create_mouse (-1, -1, x, y);
  else if (game == GAME_JOIN)
    join_mouse (-1, -1, x, y);
  else if (game == GAME_QUIT)
    quit_mouse (-1, -1, x, y);
}

static void myMouseFunc (int button, int state, int x, int y)
{
  if (game == GAME_PLAY)
  {
    game_mouse (button, state, x, y);
  }
  else if (game == GAME_INIT)
  {
    init_mouse (button, state, x, y);
  }
  else if (game == GAME_MENU)
  {
    menu_mouse (button, state, x, y);
  }
  else if (game == GAME_MISSION)
  {
    mission_mouse (button, state, x, y);
  }
  else if (game == GAME_STATS)
  {
    stats_mouse (button, state, x, y);
  }
  else if (game == GAME_FAME)
  {
    fame_mouse (button, state, x, y);
  }
  else if (game == GAME_FIGHTER)
  {
    fighter_mouse (button, state, x, y);
  }
  else if (game == GAME_CREATE)
  {
    create_mouse (button, state, x, y);
  }
  else if (game == GAME_JOIN)
  {
    join_mouse (button, state, x, y);
  }
  else if (game == GAME_QUIT)
  {
    quit_mouse (button, state, x, y);
  }
  else if (game == GAME_CREDITS)
  {
    credits_mouse (button, state, x, y);
  }
}

static void myReshapeFunc (int width, int height)
{
  ::width = width;
  ::height = height;
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_reshape ();
  else if (game == GAME_MENU || game == GAME_MISSION || game == GAME_QUIT)
    menu_reshape ();
  else if (game == GAME_INIT)
    init_reshape ();
  else if (game == GAME_CREDITS || game == GAME_FINISH)
    credits_reshape ();
  else if (game == GAME_STATS)
    stats_reshape ();
}

static void myDisplayFunc ()
{
  if (game == GAME_INIT)
  {
    init_display ();
    game_view ();
  }
  else if (game == GAME_PLAY)
  {
    game_display ();
    game_view ();
  }
  else if (game == GAME_PAUSE)
  {
    game_display ();
    pause_display ();
    game_view ();
  }
  else if (game == GAME_MENU)
  {
    if (camera == 50) camera = 0;
    game_display ();
    menu_display ();
    game_view ();
  }
  else if (game == GAME_CREDITS)
  {
    if (camera == 50) camera = 0;
    game_display ();
    credits_display ();
    game_view ();
  }
  else if (game == GAME_MISSION)
  {
    mission_display ();
    game_view ();
  }
  else if (game == GAME_STATS)
  {
    if (camera == 50) camera = 0;
    game_display ();
    stats_display ();
    game_view ();
  }
  else if (game == GAME_FAME)
  {
    fame_display ();
    game_view ();
  }
  else if (game == GAME_FIGHTER)
  {
    fighter_display ();
    game_view ();
  }
  else if (game == GAME_CREATE)
  {
    create_display ();
    game_view ();
  }
  else if (game == GAME_JOIN)
  {
    join_display ();
    game_view ();
  }
  else if (game == GAME_FINISH)
  {
    if (camera == 50) camera = 0;
    game_display ();
    finish_display ();
    game_view ();
  }
  else if (game == GAME_QUIT)
  {
    game_display ();
    quit_display ();
    game_view ();
  }
}

static void myIdleFunc ()
{
#ifdef USE_GLUT
  glutPostRedisplay ();
#endif
}

static void myJoystickAxisFunc (/*int x, int y, int t, int r*/)
{
  if (game == GAME_PLAY)
  {
    game_joystickaxis (/*x, y, t, r*/);
  }
}

static void myJoystickButtonFunc (int button)
{
  if (game == GAME_PLAY)
  {
    game_joystickbutton (button);
  }
}

static void myJoystickHatFunc (int hat)
{
  if (game == GAME_PLAY)
  {
    game_joystickhat (hat);
  }
}

static void myTimerFunc (int value)
{
  Uint32 akttime, dt;
#ifndef USE_GLUT
    akttime = SDL_GetTicks ();
#else
    akttime = glutGet (GLUT_ELAPSED_TIME);
#endif
  if (lasttime == 0) dt = 1;
  else dt = akttime - lasttime;
  lasttime = akttime;

  if (dt > 1000)
  {
    dt = 1;
    if (game == GAME_PLAY && multiplayer)
    {
      display ("Out of sync", LOG_ERROR);
      switch_menu ();
    }
  }

  if (game == GAME_PLAY)
    game_timer (dt);
  else if (game == GAME_INIT)
    init_timer (dt);
  else if (game == GAME_MENU)
  {
    if (!missionactive)
      game_timer (dt);
    menu_timer (dt);
  }
  else if (game == GAME_CREDITS)
  {
    credits_timer (dt);
  }
  else if (game == GAME_CREATE)
    create_timer (dt);
  else if (game == GAME_JOIN)
    join_timer (dt);
  else if (game == GAME_MISSION)
    mission_timer (dt);
  else if (game == GAME_STATS)
    stats_timer (dt);
  else if (game == GAME_FAME)
    mission_timer (dt);
  else if (game == GAME_FIGHTER)
    mission_timer (dt);
  else if (game == GAME_FINISH)
    finish_timer (dt);
  else if (game == GAME_QUIT)
    mission_timer (dt);

#ifdef USE_GLUT
  glutTimerFunc (1, myTimerFunc, 0); // do as many timer calls as possible
#endif
}

// sdlTimerFunc has been replaced by myTimerFunc
//#define TIMER_INTERVAL 33
//#define GLUT_TIMER_INTERVAL 28
/*static void sdlTimerFunc ()
{
  Uint32 akttime, dt;
#ifndef USE_GLUT
    akttime = SDL_GetTicks ();
#else
    akttime = glutGet (GLUT_ELAPSED_TIME);
#endif
  if (lasttime == 0) dt = 1;
  else dt = akttime - lasttime;
  lasttime = akttime;

  if (game == GAME_PLAY)
    game_timer (dt);
  else if (game == GAME_INIT)
    init_timer (dt);
  else if (game == GAME_MENU)
  {
    if (!missionactive)
      game_timer (dt);
    menu_timer (dt);
  }
  else if (game == GAME_CREDITS)
  {
    credits_timer (dt);
  }
  else if (game == GAME_CREATE)
    create_timer (dt);
  else if (game == GAME_JOIN)
    join_timer (dt);
  else if (game == GAME_MISSION)
    mission_timer (dt);
  else if (game == GAME_STATS)
    stats_timer (dt);
  else if (game == GAME_FAME)
    mission_timer (dt);
  else if (game == GAME_FIGHTER)
    mission_timer (dt);
  else if (game == GAME_FINISH)
    finish_timer (dt);
  else if (game == GAME_QUIT)
    mission_timer (dt);
}*/

#ifndef USE_GLUT

Uint32 nexttime = 0;

// get time left until nexttime, obsolete
/*Uint32 sdlTimeLeft ()
{
  Uint32 now;
  now = SDL_GetTicks ();
  if (nexttime <= now)
  {
    nexttime = now + TIMER_INTERVAL;
    return 0;
  }
  return nexttime - now;
}*/

// delay loop, not elegant => obsolete
/*void sdlWaitTimer ()
{
  Uint32 now;
  now = SDL_GetTicks ();
  if (abs (nexttime - now) > 2000) nexttime = now;
  while (now < nexttime)
  {
    now = SDL_GetTicks ();
  }
  nexttime = now + TIMER_INTERVAL;
}*/

//int joystickx = 0, joysticky = 0, joystickt = 0, joystickr = 0; // the joystick axes
int joystickbutton = -1;

// This loop emulates the glutMainLoop() of GLUT using SDL!!!
void sdlMainLoop ()
{
  SDL_Event event;
  
  while (true)
  {
    while (SDL_PollEvent (&event)) // process events
    {
      switch (event.type)
      {
        case SDL_MOUSEMOTION:
          myPassiveMotionFunc (event.motion.x, event.motion.y);
          if (game == GAME_PLAY && controls == CONTROLS_MOUSE_EXP)
          {
            fplayer->rolleffect = 0;
            fplayer->elevatoreffect = 0;
            fplayer->ruddereffect = 0;
          }
          myPassiveRelMotionFunc (event.motion.xrel, event.motion.yrel);
          break;
        case SDL_MOUSEBUTTONDOWN:
          myMouseFunc (event.button.button, event.button.state, event.button.x, event.button.y);
          break;
        case SDL_KEYDOWN:
          if (!event.key.keysym.unicode)
            mySpecialFunc (event.key.keysym.sym, 0, 0);
          else
            myKeyboardFunc (event.key.keysym.sym, 0, 0);
          break;
        case SDL_KEYUP:
          if (!event.key.keysym.unicode)
            mySpecialFuncUp (event.key.keysym.sym, 0, 0);
          else
            myKeyboardFuncUp (event.key.keysym.sym, 0, 0);
          break;
        case SDL_JOYAXISMOTION:
//          if (abs (event.jaxis.value) > 500)
          jaxis [event.jaxis.axis + event.jaxis.which * 10] = event.jaxis.value;
          if (abs (event.jaxis.value) < 1000)
            jaxis [event.jaxis.axis + event.jaxis.which * 10] = 0;
/*          {
            if (event.jaxis.axis == 0)
            {
              joystickx = event.jaxis.value;
            }
            else if (event.jaxis.axis == 1)
            {
              joysticky = event.jaxis.value;
            }
            else if (event.jaxis.axis == 2)
            {
              joystickt = event.jaxis.value;
            }
            else if (event.jaxis.axis == 3)
            {
              joystickr = event.jaxis.value;
            }
            if (joystickx > -3000 && joystickx < 3000) joystickx = 0;
            if (joysticky > -3000 && joysticky < 3000) joysticky = 0;
          }*/
          break;
        case SDL_JOYBUTTONDOWN:
          joystickbutton = event.jbutton.button + event.jbutton.which * 1000;
          myJoystickButtonFunc (joystickbutton);
          break;
        case SDL_JOYBUTTONUP:
          joystickbutton = -1;
          break;
        case SDL_JOYHATMOTION:
          myJoystickHatFunc (event.jhat.value + event.jhat.which * 1000);
          break;
        case SDL_ACTIVEEVENT:
          sdlreshape = true;
          sdldisplay = true;
          break;
      }
    }
    
    if (controls == CONTROLS_JOYSTICK)
    {
      myJoystickAxisFunc (/*joystickx, joysticky, joystickt, joystickr*/);
      if (joystickbutton == joystick_firecannon)
        myJoystickButtonFunc (joystick_firecannon);
      else
        joystickbutton = -1;
    }
    
    if (sdldisplay) myDisplayFunc ();
    sdldisplay = false;
    if (sdlreshape) myReshapeFunc (width, height);
    sdlreshape = false;
    myTimerFunc (1); // dummy value
  }
}
#endif



/****************************************************************************
  STARTUP METHODS
****************************************************************************/

// common GLUT screen init code, return 0 on error
int setGlutScreen (int w, int h, int b, int f)
{
  char gamestr [256];
  sprintf (gamestr, "%dx%d:%d", w, h, b);
  glutGameModeString (gamestr);
  if (f)
  {
    if (glutGameModeGet (GLUT_GAME_MODE_POSSIBLE))
    {
      glutEnterGameMode ();
    }
    else
    {
      return 0;
    }
  }
  else
  {
    glutInitWindowPosition (0, 0);
    glutInitWindowSize (w, h);
    glutwindow = glutCreateWindow ("GL-117");
    if (glutwindow == GL_FALSE)
      return 0;
  }
  return 1;
}

// set screen to (width, height, bpp, fullscreen), return 0 on error
int setScreen (int w, int h, int b, int f)
{

#ifdef USE_GLUT

  if (!setGlutScreen (w, h, b, f))
  {
    b = 16;
    if (!setGlutScreen (w, h, b, f))
    {
      b = 8;
      if (!setGlutScreen (w, h, b, f))
      {
        b = 2;
        if (!setGlutScreen (w, h, b, f))
        {
          return 0;
        }
      }
    }
  }

#else

  Uint32 video_flags;
  if (f)
  {
    video_flags = SDL_OPENGL | SDL_FULLSCREEN;
  }
  else
  {
    video_flags = SDL_OPENGL;
  }
  int rgb_size [3];
  switch (b)
  {
    case 8:
      rgb_size [0] = 2;
      rgb_size [1] = 3;
      rgb_size [2] = 3;
      break;
    case 15:
    case 16:
      rgb_size [0] = 5;
      rgb_size [1] = 5;
      rgb_size [2] = 5;
      break;
    default:
      rgb_size [0] = 8;
      rgb_size [1] = 8;
      rgb_size [2] = 8;
      break;
  }
  SDL_GL_SetAttribute (SDL_GL_RED_SIZE, rgb_size [0]);
  SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, rgb_size [1]);
  SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, rgb_size [2]);
  SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  if (SDL_SetVideoMode (w, h, b, video_flags) == NULL)
  {
    if ((b = SDL_VideoModeOK (w, h, b, video_flags)) != 0)
    {
      b = 16;
      SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 5);
      SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 5);
      SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 5);
      if (SDL_SetVideoMode (w, h, b, video_flags) == NULL)
      {
        b = 8;
        SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 2);
        SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 3);
        SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 3);
        if (SDL_SetVideoMode (w, h, b, video_flags) == NULL)
        {
          return 0;
        }
      }
    }
  }

#endif

  glViewport (0, 0, (GLint) w, (GLint) h);

  // take over results in global variables
  width = w;
  height = h;
  bpp = b;
  fullscreen = f;
  return 1;
}

// get approximate speed by drawing polygons (obsolete)
int speedTest ()
{
  int frames = 0, time2 = 0, time1 = 0;

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (80.0, 1.0, nearclippingplane, 20.0); // should be sqrt(2) or 1.5
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  #ifndef USE_GLUT
    time1 = SDL_GetTicks ();
  #else
    time1 = glutGet (GLUT_ELAPSED_TIME);
  #endif

  while (time2 - time1 < 1000)
  {
    frames ++;
  #ifndef USE_GLUT
    time2 = SDL_GetTicks ();
  #else
    time2 = glutGet (GLUT_ELAPSED_TIME);
  #endif

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotatef (1, 0, 0, 1);
    for (float zf = -13; zf <= -3; zf += 0.05)
    {
      glBegin (GL_QUADS);
      glColor4d (255, 0, 0, 255);
      glVertex3f (-1, -1, zf);
      glVertex3f (-1, 1, zf);
      glVertex3f (1, 1, zf);
      glVertex3f (1, -1, zf);
      glEnd ();
    }

#ifdef USE_GLUT
  glutSwapBuffers();
#else
  SDL_GL_SwapBuffers ();
#endif

  }
  return frames;
}

bool configinit = false; // has GLUT/SDL already been inited?

// test screen settings automatically
void config_test (int argc, char **argv)
{
  display ("No configuration file found. Testing...", LOG_MOST);
  char buf [STDSIZE];
  int bppi [4];

#ifdef USE_GLUT // GLUT ONLY
  display ("Using GLUT only", LOG_MOST);
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
  configinit = true;
#else // SDL
  display ("Using SDL and GLUT", LOG_MOST);
  if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
  {
    sprintf (buf, "Couldn't initialize SDL: %s", SDL_GetError ());
    display (buf, LOG_FATAL);
    exit (EXIT_INIT);
  }
  configinit = true;
#endif

  int valids = -1; // valid screen mode? (-1 = no mode)
  int n = 0;
  while (n < 4)
  {
    if (setScreen (resolution [n] [0], resolution [n] [1], resolution [n] [2], resolution [n] [3]))
    {
      bppi [n] = bpp; // store bpp setting
      valids = n;
      break; // first mode found => exit loop
    }
    else
    {
      bppi [n] = -1;
    }
    n ++;
  }

  if (valids == -1)
  {
    display ("No working display modes found! Try editing the file conf yourself. You may not be able to play this game.", LOG_FATAL);
    exit (EXIT_INIT);
  }

  // start with lowest quality/view settings
  quality = 0;
  view = 20;
  width = resolution [valids] [0];
  height = resolution [valids] [1];
  bpp = bppi [valids];
  fullscreen = resolution [valids] [3];
}

// get startup help screen
void viewParameters ()
{
  display (" ", LOG_NONE);
  display ("Usage: gl-117 [-h -v -dLEVEL]", LOG_NONE);
  display (" ", LOG_NONE);
  display ("-h: Display this help screen and quit", LOG_NONE);
  display ("-v: Display version string and quit", LOG_NONE);
  display ("-dLEVEL: Set debug LEVEL to 0=silent...5=log all", LOG_NONE);
  display (" ", LOG_NONE);
}

void checkargs (int argc, char **argv)
{
  char buf [STDSIZE]; // temp buffer
  int i; // temp counter

  for (i = 1; i < argc; i ++) // for each arg (argument/parameter)
  {
    if (argv [i] [1] == 'd') // change log/debug level
    {
      char *ptr = &argv [i] [2];
      debuglevel = atoi (ptr);
      if (debuglevel < LOG_NONE || debuglevel > LOG_ALL) // look at common.h for the constants
      {
        display ("Invalid debug level", LOG_FATAL);
        viewParameters ();
        exit (EXIT_COMMAND);
      }
      else
      {
        sprintf (buf, "Entering debug level %d", debuglevel);
        display (buf, LOG_MOST);
      }
    }
    else if (argv [i] [1] == 'v') // display version string
    {
      display (VERSIONSTRING, LOG_NONE);
      exit (EXIT_NORMAL);
    }
    else if (argv [i] [1] == 'h') // display startup help screen
    {
      viewParameters ();
      exit (EXIT_NORMAL);
    }
    else
    {
      display ("Invalid command line parameter", LOG_FATAL);
      viewParameters ();
      exit (EXIT_COMMAND);
    }
  }
}



/****************************************************************************
  GL-117 ENTRY POINT
****************************************************************************/

int main (int argc, char **argv)
{
  char buf [STDSIZE]; // temp buffer

  checkargs (argc, argv); // process command line parameters

  sprintf (buf, "Startup %s, %s ... ", argv [0], VERSIONSTRING);
  display (buf, LOG_MOST);

#ifdef _MSC_VER
  display ("Windows detected ", LOG_MOST);
#endif

  display ("Getting directory locations", LOG_ALL);
  dirs = new Dirs (argv [0]); // get data directory (DATADIR, defined via autoconf)
  
  if (!load_config ()) // try to load conf file (conf.cpp) and validate settings
  {
    // no conf file found => create new one
    display ("Creating new configuration", LOG_ALL);
    config_test (argc, argv); // do screen test
    firststart = true; // enable adjusting quality/view/graphics automatically by the game
  }

  save_config (); // save conf file (validated)

  load_configInterface (); // load interface settings from conf.interface and validate
  save_configInterface (); // save interface settings

// here srand should be called to initialize the random number generator
// this is currently done by grabbing random numbers via the init methods (not very elegant)

  server = NULL;
  client = NULL;

  display ("Creating/Loading pilots list", LOG_ALL);
  pilots = new PilotList (dirs->getSaves ("pilots")); // look at pilots.h

// NO SDL FOUND => USE GLUT ONLY
#ifdef USE_GLUT

  display ("Using GLUT only", LOG_MOST);
  if (!configinit)
  {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
    if (!setScreen (width, height, bpp, fullscreen))
    {
      sprintf (buf, "No working display mode %dx%d found", width, height);
      display (buf, LOG_FATAL);
      exit (EXIT_INIT);
    }
  }

  display ("Calling main initialization method", LOG_ALL);
  myFirstInit ();

  display ("Creating dummy sound system, install SDL to enable sound", LOG_ALL);
  sound = new SoundSystem ();

  display ("Registering GLUT callbacks", LOG_ALL);
  glutReshapeFunc (myReshapeFunc);
  glutDisplayFunc (myDisplayFunc);
  glutKeyboardFunc (myKeyboardFunc);
  glutSpecialFunc (mySpecialFunc);
  glutPassiveMotionFunc (myPassiveMotionFunc);
  glutMouseFunc (myMouseFunc);
  glutIdleFunc (myIdleFunc);
  glutTimerFunc (20, myTimerFunc, 0);

  // parameters: visible angle, aspectracio, z-nearclip, z-farclip
  gluPerspective (80.0, (float) width / height, nearclippingplane * GLOBALSCALE, 50.0 * GLOBALSCALE);
  
  // no keyboard available with GLUT, as there are no KEY_DOWN/UP events
  if (controls <= 0)
    controls = CONTROLS_MOUSE;

  display ("Entering GLUT main loop", LOG_ALL);
  glutMainLoop(); // give controls to GLUT

// SDL FOUND
#else

  display ("Using SDL and GLUT", LOG_MOST);
  if (!configinit)
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
    {
      sprintf (buf, "Couldn't initialize SDL: %s", SDL_GetError ());
      display (buf, LOG_FATAL);
      exit (EXIT_INIT);
    }
  atexit (SDL_Quit);

// SDL_NET INSTALLED?
#ifdef HAVE_SDL_NET
  if (SDLNet_Init () == -1) // initialize SDL_net
  {
    sprintf (buf, "SDLNet_Init: %s", SDLNet_GetError ());
    display (buf, LOG_FATAL);
    exit (EXIT_INIT);
  }
  display ("Using SDL_net", LOG_MOST);
#endif

  if (!configinit)
  {
    if (!setScreen (width, height, bpp, fullscreen))
    {
      sprintf (buf, "No working display mode %dx%d found", width, height);
      display (buf, LOG_FATAL);
      exit (EXIT_INIT);
    }
  }

  display ("Setting SDL caption", LOG_ALL);
  SDL_WM_SetCaption ("GL-117", "GL-117"); // window name

  SDL_ShowCursor (0);

  display ("Creating sound system", LOG_ALL);
  sound = new SoundSystem (); // look at audio.cpp
  sound->volumesound = volumesound;
  sound->volumemusic = volumemusic;
  sound->setVolume (); // set all sound volumes
  sound->setVolumeMusic (); // set all music volumes

  display ("Playing startup music", LOG_ALL);
  sound->playMusic ();

  display ("Calling main initialization method", LOG_ALL);
  myFirstInit ();
  myReshapeFunc (width, height);

  display ("Querying joystick", LOG_ALL);
  joysticks = SDL_NumJoysticks ();
  memset (jaxis, 0, maxjaxis * maxjoysticks * sizeof (int));
  if (joysticks > 0)
  {
    for (i = 0; i < joysticks; i ++)
    {
      SDL_JoystickEventState (SDL_ENABLE);
      sdljoystick [i] = SDL_JoystickOpen (i);
      sprintf (buf, "Joystick \"%s\" detected", SDL_JoystickName (i));
      display (buf, LOG_MOST);
    }
  }
  else
  {
    display ("No joystick found", LOG_MOST);
//    sdljoystick [0] = NULL;
    if (controls == CONTROLS_JOYSTICK) // no joystick available, so switch to mouse controls
      controls = CONTROLS_MOUSE;
  }

// disable joystick manually
//  joystick = 0;

  SDL_EnableUNICODE (1);
  SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

// Restrict mouse to SDL window
//  SDL_WM_GrabInput (SDL_GRAB_ON);

  display ("Entering SDL main loop (GLUT emulation)", LOG_ALL);
  sdlMainLoop (); // simulate GLUT's main loop (above)

#endif
  
  return 0; // exit without signaling errors
}

#endif
