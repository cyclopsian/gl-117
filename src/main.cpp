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

/*
TODO:
- missile countermeasure
- source code: MissionX => mission.cpp
- southern seashore landscape (additional missions)
- torpedo, water
- clouds
*/

#ifndef IS_MAIN_H

#include "main.h"

int mousex, mousey;

int quality = 3;
float view = 50.0;
int width = 800, height = 600, bpp = 32;
int dithering = 0;
int volumesound = 100;
int volumemusic = 100;

bool debug = false;
bool multiplayer = false, isserver = false;
int fullscreen = 1;
int day = 1;
int weather = WEATHER_SUNNY;
int controls = CONTROLS_MOUSE;
float sungamma = 45.0;

int camera = 0;
float camx = 0, camy = 0, camz = 0, camphi = 0, camgamma = 0, camtheta = 0;
float sunlight = 1.0;

int lighting = 1;
int mode = 0;
//int modes [4];
int resolution [4] [4] =
        { { 800, 600, 32, 1 },
          { 1024, 768, 32, 1 },
          { 640, 480, 32, 1 },
          { 640, 480, 32, 0 } };

int difficulty = 1;

float nearclippingplane = 0.1;

bool sunblinding=false;

Dirs *dirs;

Server *server;
Client *client;

#ifdef HAVE_SDL
SDL_Thread *threadnet = NULL;
#endif

int net_thread_main (void *data);

int game = GAME_INIT;

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










CTexture *texsun, *texflare1, *texflare2, *texflare3, *texflare4, *texfont1, *textfont2, *texmoon, *texcross, *texcross2, *texranks, *texmedals;

PilotList *pilots;

CExplosion *explosion [maxexplosion];
CBlackSmoke *blacksmoke [maxblacksmoke];
GLLandscape *l = NULL;
Font *font1, *font2;




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
    if (timer & 1)
    {
      buf [cursorpos] = 'I';
      buf [cursorpos + 1] = '\0';
    }
    font1->drawText (x, y, z, buf);
  }
};



class InitKugel
{
  public:
  int explosion;
  CModel *m;
  CVector3 tl;
  InitKugel(){}

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
};






/*class Fighter : public AIObj
{
  public:

  void fighterinit ()
  {
    shield = 100; maxshield = 100;
    id = FIGHTER1;
    speed = 0.3;
    recspeed = 0.3;
    smoke->type = 1;
    impact = 2;
    missiles [0] = 10;
    missiles [1] = 10;
    missiles [2] = 10;
    missiles [3] = 10;
    missiles [4] = 10;
    ttl = -1;
    easymodel = true;
    aileroneffect = 0;
    ruddereffect = 0;
  }
  Fighter ()
  {
    o = NULL;
    zoom = 1.0;
    fighterinit ();
  }
  Fighter (Space *space2, CModel *o2, float zoom2)
  {
    o = o2;
    zoom = zoom2;
    this->space = space2;
    fighterinit ();
    space->addObject (this);
  }
};*/






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
CModel model_missile1;
CModel model_missile2;
CModel model_missile3;
CModel model_missile4;
CModel model_missile5;
CModel model_missile6;
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




/*void RenderScene() 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear The Screen And The Depth Buffer
  glLoadIdentity();                  // Reset The matrix
    glShadeModel (GL_SMOOTH);
  glRotatef (-50, 1, 1, 1);

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

  // Give OpenGL our position,  then view,    then up vector
//  gluLookAt(    0, 1.5f, 8,    0, 0.5f, 0,      0, 1, 0);
  
  // We want the model to rotate around the axis so we give it a rotation
  // value, then increase/decrease it. You can rotate right of left with the arrow keys.


  // Since we know how many objects our model has, go through each of them.
//model.draw ();

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

  glutSwapBuffers ();
}
*/

/*void drawText (float x, float y, float z, char *str, CColor *c)
{
  int len = strlen (str);
  float zoom = 0.1;
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);
  gl->enableTextures (texfont1->textureID);
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glBegin (GL_QUADS);
  glColor4ub (c->c [0], c->c [1], c->c [2], c->c [3]);
  for (int i = 0; i < len; i ++)
  if (str [i] >= '-' && str [i] <= 'Z')
  {
    int c = (int) (str [i] - '-');
    if (c >= 14) c = (int) (str [i] - 'A') + 14;
    float dx = 34.0 / 256.0;
    float dy = 42.0 / 256.0, dy2 = dy;
    float tx = dx * (c % 7);
    float ty = 1.0 - dy * (c / 7);
    glTexCoord2f (tx, ty - dy2);
    glVertex3f ((x + i) * zoom, y * zoom, z);
    glTexCoord2f (tx, ty);
    glVertex3f ((x + i) * zoom, (y + 1) * zoom, z);
    glTexCoord2f (tx + dx, ty);
    glVertex3f ((x + i + 1) * zoom, (y + 1) * zoom, z);
    glTexCoord2f (tx + dx, ty - dy2);
    glVertex3f ((x + i + 1) * zoom, y * zoom, z);
  }
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  gl->disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void drawText (float x, float y, float z, char *str)
{
  CColor stdcol (255, 255, 255, 190);
  drawText (x, y, z, str, &stdcol);
}

void drawTextRotated (float x, float y, float z, char *str, CColor *col, int timer)
{
  int len = strlen (str);
  float zoom = 0.1;
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);
  gl->enableTextures (texfont1->textureID);
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  for (int i = 0; i < len; i ++)
  if (str [i] >= '-' && str [i] <= 'Z')
  {
    int c = (int) (str [i] - '-');
    if (c >= 14) c = (int) (str [i] - 'A') + 14;
    float dx = 34.0 / 256.0;
    float dy = 42.0 / 256.0, dy2 = dy;
    float tx = dx * (c % 7);
    float ty = 1.0 - dy * (c / 7);
    glPushMatrix ();
    glTranslatef ((0.5 + x + i) * zoom, (0.5 + y) * zoom, z);
    gl->rotate (timer + i * 20, 0, 0);
    glBegin (GL_QUADS);
    glColor4ub (col->c [0], col->c [1], col->c [2], col->c [3]);
    glTexCoord2f (tx, ty - dy2);
    glVertex3f (-0.5 * zoom, -0.5 * zoom, 0);
    glTexCoord2f (tx, ty);
    glVertex3f (-0.5 * zoom, 0.5 * zoom, 0);
    glTexCoord2f (tx + dx, ty);
    glVertex3f (0.5 * zoom, 0.5 * zoom, 0);
    glTexCoord2f (tx + dx, ty - dy2);
    glVertex3f (0.5 * zoom, -0.5 * zoom, 0);
    glEnd ();
    glPopMatrix ();
  }
  gl->disableAlphaBlending ();
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_TEXTURE_2D);
}*/










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

//GLLandscape *l;
CModel *obj, *objlaser, *objmissile;
CVector3 *clip1, *clip2, *tlnull, *tlinf, *tlminf;

EditField pilotedit;

GLenum polygonMode = GL_FILL;

GLfloat light_position0[] = { 0.0, 0.5, -1.0, 0 };

#define max(x,y) ((x)>(y)?(x):(y))

int NX, NY;

int joystick;

#define MISSION_DEMO 0
#define MISSION_TUTORIAL 1
#define MISSION_DOGFIGHT 2
#define MISSION_MOON 5
#define MISSION_CANYON 6
#define MISSION_TRANSPORT 10
#define MISSION_CONVOY 11
#define MISSION_DOGFIGHT2 12
#define MISSION_AIRBATTLE 13
#define MISSION_SADEFENSE 14
#define MISSION_SCOUT 15
#define MISSION_BASE 16
#define MISSION_DEFEND1 20
#define MISSION_DOGFIGHT3 21
#define MISSION_TANK1 22
#define MISSION_SHIP1 25
#define MISSION_SHIP2 26
#define MISSION_SHIP3 27
#define MISSION_CANYON1 30
#define MISSION_CANYON2 31
#define MISSION_CANYON3 32
#define MISSION_MOON1 33
#define MISSION_MOON2 34
#define MISSION_MOON3 35
#define MISSION_MULTIPLAYER 1000
#define MISSION_MULTIPLAYER_DOGFIGHT 1000

class Mission
{
  protected:
  void autoLFBriefing ()
  {
    int i, z = 0;
    for (i = 0; i < (int) strlen (briefing); i ++)
    {
      z ++;
      if (z > 50 && briefing [i] == ' ')
      {
        briefing [i] = '\n';
        z = 0;
      }
    }
  }

  public:
  int timer;
  int maxtime;
  int alliedfighters;
  char briefing [1000];
  char name [64];
  int id;
  int selfighter [3];
  int selfighters, wantfighter;
  int selweapon [3];
  int selweapons, wantweapon;
  int alliedpilot [10];
  CColor textcolor;
  int difficulty;

  Mission ()
  {
    int i;
    timer = 0;
    for (i = 0; i < 3; i ++)
      selfighter [i] = 0;
    selfighter [0] = FIGHTER_FALCON;
    selfighter [1] = FIGHTER_HAWK;
    selfighter [2] = FIGHTER_HAWK2;
    selfighters = 2;
    wantfighter = 0;
    selweapons = 3;
    selweapon [0] = MISSILE_AIR1;
    selweapon [1] = MISSILE_GROUND1;
    selweapon [2] = MISSILE_DF1;
    wantweapon = 0;
    textcolor.setColor (255, 255, 0, 128);
  }
  void playerInit ()
  {
    int i;
    fplayer = fighter [0];
    if (controls != 100)
      fplayer->easymodel = false;
    fplayer->target = NULL;
    fplayer->newinit (selfighter [wantfighter], 1, 0);
    if (selfighter [wantfighter] == FIGHTER_FALCON) fplayer->o = &model_fig;
    else if (selfighter [wantfighter] == FIGHTER_HAWK) fplayer->o = &model_figb;
    else if (selfighter [wantfighter] == FIGHTER_HAWK2) fplayer->o = &model_figc;
    else if (selfighter [wantfighter] == FIGHTER_PHOENIX) fplayer->o = &model_figf;
    else if (selfighter [wantfighter] == FIGHTER_REDARROW) fplayer->o = &model_figg;
    else if (selfighter [wantfighter] == FIGHTER_BLACKBIRD) fplayer->o = &model_figh;
    fplayer->ai = false;
    for (i = 0; i < missiletypes; i ++)
      fplayer->missiles [i] = 0;
    if (selweapon [wantweapon] == MISSILE_AIR1)
    {
      if (selfighter [wantfighter] == FIGHTER_FALCON)
      { fplayer->missiles [0] = 8; fplayer->missiles [3] = 0; }
      if (selfighter [wantfighter] == FIGHTER_HAWK)
      { fplayer->missiles [0] = 5; fplayer->missiles [3] = 2; }
      if (selfighter [wantfighter] == FIGHTER_HAWK2)
      { fplayer->missiles [0] = 5; fplayer->missiles [3] = 2; }
      if (selfighter [wantfighter] == FIGHTER_PHOENIX)
      { fplayer->missiles [2] = 4; fplayer->missiles [4] = 2; }
      if (selfighter [wantfighter] == FIGHTER_REDARROW)
      { fplayer->missiles [1] = 8; fplayer->missiles [3] = 0; }
    }
    if (selweapon [wantweapon] == MISSILE_GROUND1)
    {
      if (selfighter [wantfighter] == FIGHTER_FALCON)
      { fplayer->missiles [0] = 2; fplayer->missiles [3] = 4; }
      if (selfighter [wantfighter] == FIGHTER_HAWK)
      { fplayer->missiles [0] = 2; fplayer->missiles [3] = 6; }
      if (selfighter [wantfighter] == FIGHTER_HAWK2)
      { fplayer->missiles [0] = 2; fplayer->missiles [4] = 6; }
      if (selfighter [wantfighter] == FIGHTER_PHOENIX)
      { fplayer->missiles [2] = 1; fplayer->missiles [4] = 8; }
      if (selfighter [wantfighter] == FIGHTER_REDARROW)
      { fplayer->missiles [1] = 2; fplayer->missiles [4] = 4; }
    }
    if (selweapon [wantweapon] == MISSILE_DF1)
    {
      if (selfighter [wantfighter] == FIGHTER_FALCON)
      { fplayer->missiles [5] = 3; }
      if (selfighter [wantfighter] == FIGHTER_HAWK)
      { fplayer->missiles [3] = 2; fplayer->missiles [5] = 6; }
      if (selfighter [wantfighter] == FIGHTER_HAWK2)
      { fplayer->missiles [4] = 2; fplayer->missiles [5] = 6; }
      if (selfighter [wantfighter] == FIGHTER_PHOENIX)
      { fplayer->missiles [4] = 4; fplayer->missiles [5] = 6; }
      if (selfighter [wantfighter] == FIGHTER_REDARROW)
      { fplayer->missiles [5] = 3; }
    }
  }
  void alliedInit (int fighterid, int pilotid, AIObj *aiobj)
  {
    Pilot *p = pilots->pilot [pilots->aktpilot];
    aiobj->easymodel = true;
    aiobj->target = NULL;
    int intelligence = p->tp [pilotid]->intelligence;
    int precision = p->tp [pilotid]->precision;
    int aggressivity = p->tp [pilotid]->aggressivity;
    aiobj->newinit (fighterid, 1, intelligence, precision, aggressivity);
    if (fighterid == FIGHTER_FALCON) aiobj->o = &model_fig;
    else if (fighterid == FIGHTER_HAWK) aiobj->o = &model_figb;
    else if (fighterid == FIGHTER_HAWK2) aiobj->o = &model_figc;
    else if (fighterid == FIGHTER_PHOENIX) aiobj->o = &model_figf;
    else if (fighterid == FIGHTER_REDARROW) aiobj->o = &model_figg;
    aiobj->ai = true;
  }
  void init ()
  {
  }
  virtual void start ()
  {
  }
  virtual int processtimer ()
  {
    return 0;
  }
  virtual void draw ()
  {
  }
  void checkScore (int missionstate, int timebonus, int fighterkills, int shipkills, int tankkills, int otherkills, int shieldbonus, int points)
  {
    Pilot *p = pilots->pilot [pilots->aktpilot];
    int i, i2;
    int oldscore = getScore (p->mission_state [id], p->mission_time [id], p->mission_fighterkills [id], p->mission_shipkills [id], p->mission_tankkills [id], p->mission_otherkills [id], p->mission_shield [id], p->mission_points [id]);
    int newscore = getScore (missionstate, timebonus, fighterkills, shipkills, tankkills, otherkills, shieldbonus, points);
    if (newscore > oldscore)
    {
      if (missionstate != p->mission_state [id])
      {
        for (i = 0; i < 10; i ++)
        {
          bool b = false;
          for (i2 = 0; i2 < alliedfighters - 1; i2 ++)
          {
            if (i == alliedpilot [i2])
            { b = true; break; }
          }
          if (b) p->tp [i]->fighterkills += fighter [i + 1]->fighterkills;
          else p->tp [i]->flyMission (myrandom (5));
        }
      }
      p->mission_state [id] = missionstate;
      p->mission_time [id] = timebonus;
      p->mission_fighterkills [id] = fighterkills;
      p->mission_shipkills [id] = shipkills;
      p->mission_tankkills [id] = tankkills;
      p->mission_otherkills [id] = otherkills;
      p->mission_shield [id] = shieldbonus;
      p->mission_points [id] = points;
      p->mission_score [id] = newscore;
      p->save ();
    }
  }
  int getScore (int missionstate, int timebonus, int fighterkills, int shipkills, int tankkills, int otherkills, int shieldbonus, int points)
  {
    int score = timebonus + shieldbonus + fighterkills * 20 + shipkills * 12 + tankkills * 10 + otherkills * 5 + points / 20;
    if (missionstate == 2) // if failed
    {
      score /= 4;
      score -= 100;
    }
    return score;
  }
  int getScore (int missionstate)
  {
    int shieldbonus = fplayer->shield * 100 / fplayer->maxshield;
    int timebonus = 0;
    if (timer < maxtime)
      timebonus = (maxtime - timer) * 100 / maxtime;
    checkScore (missionstate, timebonus, fplayer->fighterkills, fplayer->shipkills, fplayer->tankkills, fplayer->otherkills, shieldbonus, fplayer->points);
    return getScore (missionstate, timebonus, fplayer->fighterkills, fplayer->shipkills, fplayer->tankkills, fplayer->otherkills, shieldbonus, fplayer->points);
  }
};

class MissionDemo1 : public Mission
{
  public:
  
  MissionDemo1 ()
  {
    id = MISSION_DEMO;
    strcpy (name, "DEMO");
    alliedfighters = 2;
  }

  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 5;
    sungamma = 45;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
    fplayer = fighter [0];
    fplayer->tl->x = 220;
    fplayer->tl->z = -30;
    fplayer->target = fighter [2];
    fplayer->newinit (FIGHTER_FALCON, 1, 0);
    fighter [1]->target = fighter [3];
    fighter [1]->o = &model_fig;
    fighter [1]->tl->x = 49;
    fighter [1]->tl->z = -30;
    fighter [1]->newinit (FIGHTER_FALCON, 1, 0);
    for (i = 2; i <= 8; i ++)
    {
      fighter [i]->target = fighter [myrandom (2)];
      fighter [i]->o = &model_figa;
      fighter [i]->newinit (FIGHTER_SWALLOW, 0, 400 - i * 20);
    }
  }
  
  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 8; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }
  
  virtual void draw ()
  {
  }
};

class MissionTutorial1 : public Mission
{
  private:
  bool loop;

  public:
  
  MissionTutorial1 ()
  {
    id = MISSION_TUTORIAL;
    strcpy (name, "TUTORIAL");
    alliedfighters = 1;
    strcpy (briefing, "WELCOME TO THE EAGLE SQUADRON! BEFORE FLYING ANY MISSION YOU SHOULD PLAY THIS TUTORIAL. LEARN TO HANDLE YOUR FIGHTER AND THE BASIC CONTROLS LIKE TARGETING OR FIRING A MISSILE.");
    autoLFBriefing ();
    maxtime = 2500;
  }

  virtual void start ()
  {
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 50;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_LOW_ALPINE, NULL);
//    l = new GLLandscape (space, LANDSCAPE_CANYON, NULL);
    playerInit ();
    fplayer->tl->x = 220;
    fplayer->tl->z = -30;
    loop = false;
  }

  virtual int processtimer ()
  {
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
      return 2;
    if (!fighter [1]->active && fighter [1]->explode >= 35 && !fighter [2]->active && fighter [2]->explode >= 35)
      return 1;
    return 0;
  }

  virtual void draw ()
  {
    int timeroff = 100, timerdelay = 300;
    if (timer >= 0 && timer <= timeroff - 20)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
    else if (timer > timeroff && timer <= timeroff + timerdelay - 20)
    {
      if (controls == 2)
      {
        font1->drawTextCentered (0, 7, -2.5, "LEAVE THE JOYSTICK CENTERED", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "THE FIGHTER SHOULD FLY STRAIGHT ON", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "IF NOT, RECALIBRATE YOUR JOYSTICK", &textcolor);
      }
      else if (controls == 1)
      {
        font1->drawTextCentered (0, 7, -2.5, "THE MENU OPTIONS/CONTROLS LETS YOU", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "DETERMINE THE INPUT DEVICE.", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "CURRENTLY, THIS IS THE MOUSE", &textcolor);
      }
      else if (controls == 0)
      {
        font1->drawTextCentered (0, 7, -2.5, "THE MENU OPTIONS/CONTROLS LETS YOU", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "DETERMINE THE INPUT DEVICE.", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "CURRENTLY, THIS IS THE KEYBOARD", &textcolor);
      }
/*      if (!loop) fplayer->gamma += 2;
      else fplayer->gamma -= 2;
      if (fplayer->checkLooping ()) loop = !loop;*/
    }
    else if (timer > timeroff + timerdelay && timer <= timeroff + 2 * timerdelay - 20)
    {
      if (controls == 2)
      {
        font1->drawTextCentered (0, 7, -2.5, "MOVING THE JOYSTICK LEFT OR RIGHT", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "WILL AFFECT THE AILERON", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "THUS YOU WILL ONLY ROLL", &textcolor);
      }
      else if (controls == 1)
      {
        font1->drawTextCentered (0, 7, -2.5, "THE MOUSE INTERFACE IS VERY EASY", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "ALWAYS POINT TO WHERE YOU WANT TO FLY", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "YOU'LL HAVE TO MOVE THE MOUSE PERMANENTLY", &textcolor);
      }
      else if (controls == 0)
      {
        font1->drawTextCentered (0, 7, -2.5, "THE KEYBOARD INTERFACE IS NOT SO EASY", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "USING THE MOUSE IS STRONLY RECOMMENDED", &textcolor);
      }
    }
    else if (timer > timeroff + 2 * timerdelay && timer <= timeroff + 3 * timerdelay - 20)
    {
      if (controls == 2)
      {
        font1->drawTextCentered (0, 7, -2.5, "MOVING UP OR DOWN WILL AFFECT", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "THE ELEVATOR", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "YOU'LL START TO FLY A LOOP", &textcolor);
      }
      else if (controls == 1 || controls == 0)
      {
        font1->drawTextCentered (0, 7, -2.5, "MOVING RIGHT OR LEFT WILL AFFECT THE AILERON", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "MOVING UP OR DOWN WILL AFFECT THE ELEVATOR", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "THUS YOU CAN FLY ROLLS AND LOOPS", &textcolor);
      }
    }
    else if (timer > timeroff + 3 * timerdelay && timer <= timeroff + 4 * timerdelay - 20)
    {
      if (controls == 2)
      {
        font1->drawTextCentered (0, 7, -2.5, "IF YOUR JOYSTICK HAS A SO-CALLED \"RUDDER\",", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "YOU MAY ALTER THE FIGHTER'S RUDDER", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "YOU'LL SLIGHTLY FLY TO THE LEFT OR RIGHT", &textcolor);
      }
      else if (controls == 1)
      {
        font1->drawTextCentered (0, 7, -2.5, "MOVING THE MOUSE SLIGHTLY LEFT OR RIGHT", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "WILL AFFECT THE RUDDER.", &textcolor);
      }
      else if (controls == 0)
      {
        font1->drawTextCentered (0, 7, -2.5, "TO ALTER THE RUDDER", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "USE THE KEYS PGUP AND PGDN", &textcolor);
      }
    }
    else if (timer > timeroff + 4 * timerdelay && timer <= timeroff + 5 * timerdelay - 20)
    {
      if (controls == 2)
      {
        font1->drawTextCentered (0, 7, -2.5, "YOUR JOYSTICK HAS ALSO A THROTTLE", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "TO SPEEDUP OR DECELERATE", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "THERE IS NO WAY TO ENTIRELY STOP", &textcolor);
      }
      else
      {
        font1->drawTextCentered (0, 7, -2.5, "USE THE KEYS '1' THROUGH '9' TO ALTER", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "YOUR THROTTLE (SPEED)", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "THERE IS NO WAY TO ENTIRELY STOP", &textcolor);
      }
    }
    else if (timer > timeroff + 5 * timerdelay && timer <= timeroff + 6 * timerdelay - 20)
    {
      if (controls == 2)
      {
        font1->drawTextCentered (0, 7, -2.5, "THE FIRST FOUR JOYSTICK BUTTONS:", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "1: FIRE CANNON, 2: TARGET NEAREST ENEMY", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "3: FIRE MISSILE, 4: CHOOSE MISSILE", &textcolor);
      }
      else if (controls == 1)
      {
        font1->drawTextCentered (0, 7, -2.5, "LEFT MOUSE BUTTON: FIRE CANNON", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "RIGHT MOUSE BUTTON: FIRE MISSILE", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "BUT YOU WILL ALSO HAVE TO USE THE KEYBOARD", &textcolor);
      }
      else if (controls == 0)
      {
        font1->drawTextCentered (0, 6, -2.5, "NOW, LETS HAVE A LOOK AT SOME IMPORTANT KEYS", &textcolor);
      }
    }
    else if (timer > timeroff + 6 * timerdelay && timer <= timeroff + 7 * timerdelay - 20)
    {
      if (controls == 2)
      {
        font1->drawTextCentered (0, 7, -2.5, "A SUMMARY OF COMMANDS CAN BE FOUND IN THE MENU", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "\"ESC\" WILL SHOW THE MENU", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "AND RETURN BACK TO THE ACTION", &textcolor);
      }
      else
      {
        font1->drawTextCentered (0, 7, -2.5, "SPACE: FIRE CANNON (UNLIMITED AMMO)", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "M: CHOOSE MISSILE", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "ENTER: FIRE MISSILE", &textcolor);
      }
    }
    else if (timer > timeroff + 7 * timerdelay && timer <= timeroff + 8 * timerdelay - 20)
    {
      font1->drawTextCentered (0, 7, -2.5, "LOOK AT THE RADAR ON THE BOTTOM OF YOUR SCREEN", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "THERE ARE ENEMIES REPRESENTED BY A YELLOW POINT (TARGETED)", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "AND A RED POINT (NOT TARGETED)", &textcolor);
    }
    else if (timer > timeroff + 8 * timerdelay && timer <= timeroff + 9 * timerdelay - 20)
    {
      if (controls == 2)
      {
        font1->drawTextCentered (0, 7, -2.5, "YOU MAY TARGET AN ENEMY USING THE COOLIE HAT,", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "BUTTON2, OR PRESSING 'T' ON THE KEYBOARD", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "THEN APPROACH!", &textcolor);
      }
      else
      {
        font1->drawTextCentered (0, 7, -2.5, "YOU MAY TARGET AN ENEMY PRESSING", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "'T' ON THE KEYBOARD", &textcolor);
        font1->drawTextCentered (0, 5, -2.5, "THEN APPROACH!", &textcolor);
      }
    }
    else if (timer > timeroff + 9 * timerdelay && timer <= timeroff + 10 * timerdelay - 20)
    {
      font1->drawTextCentered (0, 7, -2.5, "SHOOT THE TARGETS TO COMPLETE", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "THIS TUTORIAL SUCCESSFULLY", &textcolor);
    }
    if (timer == timeroff + 7 * timerdelay - 10)
    {
      fighter [1]->activate ();
      fighter [1]->target = fighter [0];
      fighter [1]->o = &model_figt;
      fighter [1]->newinit (FIGHTER_TRANSPORT, 0, 200);
      fighter [1]->tl->x = fplayer->tl->x - 30;
      fighter [1]->tl->z = fplayer->tl->z - 30;
      fighter [2]->activate ();
      fighter [2]->target = fighter [0];
      fighter [2]->o = &model_figt;
      fighter [2]->newinit (FIGHTER_TRANSPORT, 0, 200);
      fighter [2]->tl->x = fplayer->tl->x + 30;
      fighter [2]->tl->z = fplayer->tl->z + 30;
    }
  }
};

class MissionDogfight1 : public Mission
{
  public:
  int state, laststate, texttimer;

  MissionDogfight1 ()
  {
    id = MISSION_DOGFIGHT;
    strcpy (name, "DOGFIGHT");
    strcpy (briefing, "HERE IS THE SECOND TASK: PRACTICE YOUR DOGFIGHT SKILLS.");
    autoLFBriefing ();
    alliedfighters = 1;
    maxtime = 5000;
  }
    
  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 25;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
    playerInit ();
    fplayer->tl->x = 20;
    fplayer->tl->z = 50;
    for (i = 1; i <= 6; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [0];
      fighter [i]->o = &model_fige;
      fighter [i]->tl->x = -i * 10;
      fighter [i]->tl->z = -i * 10;
      fighter [i]->newinit (FIGHTER_CROW, 0, 400);
      fighter [i]->deactivate ();
    }
    fighter [1]->activate ();
    state = 0;
    laststate = 0;
    texttimer = 0;
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 6; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    state ++;
    if (state == 1)
    {
      for (i = 2; i <= 3; i ++)
      {
        fighter [i]->activate ();
        fighter [i]->tl->x = fplayer->tl->x + 50 + 10 * i;
        fighter [i]->tl->z = fplayer->tl->z + 50 + 10 * i;
        fighter [i]->tl->y = l->getHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 15;
      }
      return 0;
    }
    else if (state == 2)
    {
      for (i = 4; i <= 6; i ++)
      {
        fighter [i]->activate ();
        fighter [i]->tl->x = fplayer->tl->x + 50 + 10 * i;
        fighter [i]->tl->z = fplayer->tl->z + 50 + 10 * i;
        fighter [i]->tl->y = l->getHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 15;
      }
      return 0;
    }
    return 1;
  }

  virtual void draw ()
  {
    if (laststate != state)
    {
      texttimer = 1;
      laststate = state;
    }
    if (texttimer >= 200) texttimer = 0;
    if (texttimer > 0)
    {
      texttimer ++;
      if (state == 1)
      {
        font1->drawTextCentered (0, 7, -2.5, "THAT WAS EASY", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "BUT CAN YOU HANDLE TWO ENEMIES", &textcolor);
      }
      if (state == 2)
      {
        font1->drawTextCentered (0, 7, -2.5, "OK, LET'S TRY THREE ENEMIES", &textcolor);
        font1->drawTextCentered (0, 6, -2.5, "THIS SHOULD BE ENOUGH", &textcolor);
      }
    }
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
    if (timer >= 70 && timer <= 220)
    {
      font1->drawTextCentered (0, 7, -2.5, "THERE IS AN ENEMY ATTACKING", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "CHOOSE AN AIR-AIR MISSILE AND FIRE", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "OR JUST USE THE CANNON", &textcolor);
    }
  }
};

class MissionTransport : public Mission
{
  public:

  MissionTransport ()
  {
    id = MISSION_TRANSPORT;
    strcpy (name, "TRANSPORT");
    strcpy (briefing, "EUROPE: THIS IS YOUR FIRST MISSION FOR THE EAGLE SQADRON. OUR SECRET SERVICE HAS CAUGHT TWO TRANSPORTS PROVIDING TERRORISTS WITH AMMUNITION. THEY ARE ONLY LIGHTLY GUARDED AND THERE SHOULD NOT BE A PROBLEM TAKING THEM OUT WITH OUR SUPERIOR FIGHTERS.");
    autoLFBriefing ();
    alliedfighters = 2;
    maxtime = 5000;
    alliedpilot [0] = PILOT_PRIMETIME;
  }

  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 45;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_LOW_ALPINE, NULL);
    playerInit ();
    fplayer->tl->x = 0;
    fplayer->tl->z = 100;
    alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = 5;
    fighter [1]->tl->z = 105;
    for (i = 2; i <= 3; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [0];
      fighter [i]->o = &model_fige;
      fighter [i]->tl->x = -i * 10;
      fighter [i]->tl->z = -i * 10;
      fighter [i]->newinit (FIGHTER_CROW, 0, 400);
    }
    for (i = 4; i <= 5; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [0];
      fighter [i]->o = &model_figt;
      fighter [i]->tl->x = -i * 10;
      fighter [i]->tl->z = -i * 10;
      fighter [i]->newinit (FIGHTER_TRANSPORT, 0, 200);
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 8; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionConvoy : public Mission
{
  public:

  MissionConvoy ()
  {
    id = MISSION_CONVOY;
    strcpy (name, "CONVOY");
    strcpy (briefing, "WE HAVE SPOTTED AN ENEMY CONVOY. THIS IS YOUR NEXT TARGET. BE CAREFUL, THERE ARE WIESEL TANKS WITH SURFACE-AIR CANNONS.");
    autoLFBriefing ();
    alliedfighters = 2;
    maxtime = 5000;
    alliedpilot [0] = PILOT_PRIMETIME;
  }
    
  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 45;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_ALPINE_EROSION, NULL);
    int px, py;
    l->searchPlain (-1, -1, &px, &py);
    px = px - MAXX / 2;
    py = MAXX / 2 - py;
//    if (px < 0) px += MAXX;
//    if (py < 0) py += MAXX;
    playerInit ();
    fplayer->tl->x = px;
    fplayer->tl->z = py + 100;
    alliedInit (FIGHTER_HAWK, alliedpilot [0], fighter [1]); // always match together 0<->1, 1<->2 etc.
    fighter [1]->tl->x = px + 5;
    fighter [1]->tl->z = py + 105;
    for (i = 2; i <= 3; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [0];
      fighter [i]->o = &model_tank1;
      int phi = (i - 2) * 180;
      fighter [i]->newinit (TANK_AIR1, 0, 200);
      fighter [i]->tl->x = px + sine [phi] * 3.0;
      fighter [i]->tl->z = py + cosi [phi] * 3.0;
      fighter [i]->phi = 359 - phi;
      fighter [i]->speed = 0;
      fighter [i]->maxspeed = 0;
    }
    for (i = 4; i <= 9; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [0];
      if (i == 6 || i == 9) fighter [i]->o = &model_pickup1;
      else fighter [i]->o = &model_truck1;
      fighter [i]->tl->x = px + 7.5 - i;
      fighter [i]->tl->z = py + 7.5 - i;
      if (i == 6 || i == 9) fighter [i]->newinit (TANK_PICKUP1, 0, 400);
      else fighter [i]->newinit (TANK_TRUCK1, 0, 400);
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 9; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionDogfight2 : public Mission
{
  public:

  MissionDogfight2 ()
  {
    id = MISSION_DOGFIGHT2;
    strcpy (name, "DOGFIGHT");
    strcpy (briefing, "SOME ENEMY FIGHTERS ARE ON THEIR WAY ATTACKING ONE OF OUR OUTPOSTS. STOP THEM BEFORE THEY CAN REACH IT!");
    autoLFBriefing ();
    alliedfighters = 3;
    maxtime = 5000;
    alliedpilot [0] = PILOT_PRIMETIME;
    alliedpilot [1] = PILOT_SHADOW;
  }

  virtual void start ()
  {
    int i;
    day = 0;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 40;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
    playerInit ();
    fplayer->tl->x = 0;
    fplayer->tl->z = 100;
    alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = 5;
    fighter [1]->tl->z = 105;
    alliedInit (FIGHTER_FALCON, alliedpilot [1], fighter [2]);
    fighter [2]->tl->x = 10;
    fighter [2]->tl->z = 110;
    for (i = 3; i <= 6; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [0];
      fighter [i]->o = &model_fige;
      fighter [i]->tl->x = -i * 10;
      fighter [i]->tl->z = -i * 10;
      fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 20);
    }
    for (i = 7; i <= 8; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [0];
      fighter [i]->o = &model_fige;
      fighter [i]->tl->x = -i * 10;
      fighter [i]->tl->z = -i * 10;
      fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 20);
      fighter [i]->deactivate ();
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 8; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
    if (timer == 1000)
    {
      fighter [7]->activate ();
      fighter [8]->activate ();
      fighter [7]->tl->x = fplayer->tl->x + 50;
      fighter [7]->tl->z = fplayer->tl->z + 50;
      fighter [7]->tl->y = l->getHeight (fighter [7]->tl->x, fighter [7]->tl->z) + 10;
      fighter [8]->tl->x = fplayer->tl->x + 60;
      fighter [8]->tl->z = fplayer->tl->z + 60;
      fighter [8]->tl->y = l->getHeight (fighter [8]->tl->x, fighter [8]->tl->z) + 10;
    }
    if (timer >= 1000 && timer <= 1200)
      font1->drawTextCentered (0, 7, -3, "MORE ENEMIES ENTERING THE REGION", &textcolor);
  }
};

class MissionAirBattle : public Mission
{
  public:

  MissionAirBattle ()
  {
    id = MISSION_AIRBATTLE;
    strcpy (name, "AIR BATTLE");
    strcpy (briefing, "WE ARE BEING ATTACKED BY A MASS OF CROWS. THE SQUADRON IS ORDERED TO DEFEND.");
    autoLFBriefing ();
    alliedfighters = 7;
    maxtime = 5000;
    alliedpilot [0] = PILOT_PRIMETIME;
    alliedpilot [1] = PILOT_SHADOW;
    alliedpilot [2] = PILOT_FIREBIRD;
    alliedpilot [3] = PILOT_HEPTARGON;
    alliedpilot [4] = PILOT_LARA;
    alliedpilot [5] = PILOT_DRDOOM;
  }

  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 45;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
    playerInit ();
    fplayer->tl->x = 0;
    fplayer->tl->z = 100;
    for (i = 1; i <= 6; i ++)
    {
      alliedInit (FIGHTER_FALCON, alliedpilot [i - 1], fighter [i]);
      if (i % 2)
        fighter [i]->tl->x = ((i + 1) / 2) * 5;
      else
        fighter [i]->tl->x = -((i + 1) / 2) * 5;
      fighter [i]->tl->z = 100 + i * 5;
      fighter [i]->target = fighter [7 + i];
    }
    for (i = 7; i <= 22; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (7)];
      fighter [i]->o = &model_fige;
      if (i <= 13)
      {
        fighter [i]->tl->x = -i * 5;
        fighter [i]->tl->z = -i * 5;
      }
      else
      {
        fighter [i]->tl->x = -i * 8 - 150;
        fighter [i]->tl->z = -i * 8 - 150;
      }
      fighter [i]->newinit (FIGHTER_CROW, 0, 450 - i * 14);
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 22; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionGround1 : public Mission
{
  public:

  MissionGround1 ()
  {
    id = MISSION_SADEFENSE;
    strcpy (name, "SA DEFENSE");
    strcpy (briefing, "OUR ENEMY HAS BUILT UP A BASE AND SOME AIR DEFENCES AROUND. THE PILOTS OF OUR SQUADRON HAVE TO TAKE OUT THESE AIR DEFENCES.");
    autoLFBriefing ();
    alliedfighters = 2;
    alliedpilot [0] = PILOT_PRIMETIME;
    maxtime = 2500;
  }

  virtual void start ()
  {
    int i;
    day = 0;
    weather = WEATHER_THUNDERSTORM;
    camera = 0;
    sungamma = 40;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_ALPINE_NOLAKE, NULL);
    int px, py;
    l->searchPlain (1, 1, &px, &py);
    l->gauss (px, py);
    px = px - MAXX / 2;
    py = MAXX / 2 - py;
    playerInit ();
    fplayer->tl->x = px + 10;
    fplayer->tl->z = py + 80;
    fplayer->target = fighter [2];
    alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = px + 20;
    fighter [1]->tl->z = py + 90;
    fighter [1]->target = fighter [2];
    for (i = 2; i <= 4; i ++)
    {
      fighter [i]->tl->x = px - 3 + i * 3;
      fighter [i]->tl->z = py + 2;
      fighter [i]->target = fighter [0];
      if (i == 2)
      {
        fighter [i]->o = &model_flarak1;
        fighter [i]->newinit (FLARAK_AIR1, 0, 220);
      }
      else
      {
        fighter [i]->o = &model_flak1;
        fighter [i]->newinit (FLAK_AIR1, 0, 200);
      }
    }
    l->searchPlain (2, 1, &px, &py);
    l->gauss (px, py);
    px = px - MAXX / 2;
    py = MAXX / 2 - py;
    for (i = 5; i <= 6; i ++)
    {
      fighter [i]->tl->x = px - 3 + i * 3;
      fighter [i]->tl->z = py + 2;
      fighter [i]->target = fighter [0];
      fighter [i]->o = &model_flak1;
      fighter [i]->newinit (FLAK_AIR1, 0, 200);
    }
    fighter [7]->tl->x = px + 1;
    fighter [7]->tl->z = py - 1;
    fighter [7]->o = &model_tent1;
    fighter [7]->newinit (STATIC_TENT1, 0, 200);
    fighter [8]->tl->x = px - 1;
    fighter [8]->tl->z = py - 1;
    fighter [8]->o = &model_tent1;
    fighter [8]->newinit (STATIC_TENT1, 0, 200);
/*      fighter [2]->activate ();
      fighter [2]->intelligence = 300;
      fighter [2]->party = 0;
      fighter [2]->target = fighter [0];
      fighter [2]->o = &model_figa;
      fighter [3]->activate ();
      fighter [3]->intelligence = 150;
      fighter [3]->party = 0;
      fighter [3]->target = fighter [0];
      fighter [3]->o = &model_figa;*/
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 6; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionScout : public Mission
{
  public:

  MissionScout ()
  {
    id = MISSION_SCOUT;
    strcpy (name, "SCOUTS");
    strcpy (briefing, "ENEMY SCOUTS ARE APPROACHING THIS REGION. BE CARFUL, THEY ARE FLYING BUZZARDS AND SHOULD BE CONSIDERED EQUAL TO OUR FALCONS.");
    autoLFBriefing ();
    alliedfighters = 2;
    maxtime = 3000;
    alliedpilot [0] = PILOT_PRIMETIME;
  }

  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 25;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
    playerInit ();
    fplayer->tl->x = 0;
    fplayer->tl->z = 100;
    alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = 5;
    fighter [1]->tl->z = 105;
    for (i = 2; i <= 3; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (2)];
      fighter [i]->o = &model_figd;
      fighter [i]->tl->x = -i * 10;
      fighter [i]->tl->z = -i * 10;
      fighter [i]->newinit (FIGHTER_BUZZARD, 0, 100 + myrandom (50));
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 8; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionBase : public Mission
{
  public:

  MissionBase ()
  {
    id = MISSION_BASE;
    strcpy (name, "BASE");
    strcpy (briefing, "NOW IT IS TIME TO STRIKE AGAINST THE TERRORISTS BASE.");
    autoLFBriefing ();
    alliedfighters = 4;
    alliedpilot [0] = PILOT_PRIMETIME;
    alliedpilot [1] = PILOT_SHADOW;
    alliedpilot [2] = PILOT_HEPTARGON;
    alliedpilot [3] = PILOT_DRDOOM;
    maxtime = 4000;
  }

  virtual void start ()
  {
    int i, i2;
    day = 0;
    weather = WEATHER_THUNDERSTORM;
    camera = 0;
    sungamma = 40;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_ALPINE_EROSION, NULL);
    int px, py;
    l->searchPlain (-1, -1, &px, &py);
    for (i = px - 6; i <= px + 6; i ++)
      for (i2 = py - 8; i2 <= py + 7; i2 ++)
      {
        l->f [i] [i2] = GREYSAND;
        int sum = (int) l->r [i] [i2] + l->g [i] [i2] + l->b [i] [i2];
        sum /= 3;
        l->r [i] [i2] = sum + 15;
        l->g [i] [i2] = sum + 15;
        l->b [i] [i2] = sum - 15;
      }
    px = px - MAXX / 2;
    py = MAXX / 2 - py;
    playerInit ();
    fplayer->tl->x = px + 10;
    fplayer->tl->z = py + 100;
    fplayer->target = fighter [4];
    for (i = 1; i <= 3; i ++)
    {
      alliedInit (FIGHTER_FALCON, alliedpilot [i - 1], fighter [i]);
      fighter [i]->tl->x = px + 10 + i * 5;
      fighter [i]->tl->z = py + 100 + i * 5;
    }
    int n = 4;
    fighter [n]->tl->x = px - 4;
    fighter [n]->tl->z = py;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_hall2;
    fighter [n]->newinit (STATIC_HALL2, 0, 400);
    n ++;
    fighter [n]->tl->x = px + 2;
    fighter [n]->tl->z = py + 5;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_hall2;
    fighter [n]->newinit (STATIC_HALL2, 0, 400);
    n ++;
    fighter [n]->tl->x = px;
    fighter [n]->tl->z = py - 1;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_hall1;
    fighter [n]->newinit (STATIC_HALL1, 0, 400);
    n ++;
    fighter [n]->tl->x = px + 2;
    fighter [n]->tl->z = py - 1;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_hall1;
    fighter [n]->newinit (STATIC_HALL1, 0, 400);
    n ++;
    fighter [n]->tl->x = px + 4;
    fighter [n]->tl->z = py - 1;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_hall1;
    fighter [n]->newinit (STATIC_HALL1, 0, 400);
    n ++;
    fighter [n]->tl->x = px - 1.5;
    fighter [n]->tl->z = py - 4.5;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_tent4;
    fighter [n]->newinit (STATIC_TENT4, 0, 400);
    n ++;
    fighter [n]->tl->x = px + 1.5;
    fighter [n]->tl->z = py - 4.5;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_tent4;
    fighter [n]->newinit (STATIC_TENT4, 0, 400);
    n ++;
    fighter [n]->tl->x = px - 3;
    fighter [n]->tl->z = py + 4;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_tent4;
    fighter [n]->newinit (STATIC_TENT4, 0, 400);
    n ++;
    fighter [n]->tl->x = px - 5.5;
    fighter [n]->tl->z = py + 4;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_tent4;
    fighter [n]->newinit (STATIC_TENT4, 0, 400);
    n ++;
    fighter [n]->tl->x = px - 4;
    fighter [n]->tl->z = py + 6;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_tent4;
    fighter [n]->newinit (STATIC_TENT4, 0, 400);
    n ++;
    fighter [n]->tl->x = px - 6;
    fighter [n]->tl->z = py + 6;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_flak1;
    fighter [n]->newinit (FLAK1, 0, 200);
    fighter [n]->phi = 90;
    fighter [n]->maxtheta = 0;
    n ++;
    fighter [n]->tl->x = px - 6;
    fighter [n]->tl->z = py - 6;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_flak1;
    fighter [n]->newinit (FLAK1, 0, 200);
    fighter [n]->phi = 0;
    fighter [n]->maxtheta = 0;
    n ++;
    fighter [n]->tl->x = px + 6;
    fighter [n]->tl->z = py - 6;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_flak1;
    fighter [n]->newinit (FLAK1, 0, 200);
    fighter [n]->phi = 270;
    fighter [n]->maxtheta = 0;
    n ++;
    fighter [n]->tl->x = px + 6;
    fighter [n]->tl->z = py + 6;
    fighter [n]->target = fighter [0];
    fighter [n]->o = &model_flak1;
    fighter [n]->newinit (FLAK1, 0, 200);
    fighter [n]->phi = 180;
    fighter [n]->maxtheta = 0;
    n ++;
    for (i = n; i < n + 6; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (4)];
      fighter [i]->o = &model_fige;
      fighter [i]->tl->x = px + i * 5 - 60;
      fighter [i]->tl->z = py + i * 5 - 60;
      fighter [i]->newinit (FIGHTER_CROW, 0, 180 + myrandom (200));
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 19; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionDefend1 : public Mission
{
  public:

  MissionDefend1 ()
  {
    id = MISSION_DEFEND1;
    strcpy (name, "DEFEND SAM");
    strcpy (briefing, "THERE ARE NEW TERRORIST ACTIVITIES IN NORTHERN AFRICA. OUR SQUARDRON HAS TO MOVE THERE AND OUR FIRST MISSION IS THE DEFENDANCE OF SOME SAM LAUNCHERS AGAINST SUPERIOR TANKS. HOWEVER, OUR OLD HAWKS HAVE BEEN REPLACED BY A NEW MODEL.");
    autoLFBriefing ();
    alliedfighters = 2;
    alliedpilot [0] = PILOT_SHADOW;
    maxtime = 3000;
    selfighter [1] = FIGHTER_HAWK2;
  }

  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 45;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_DESERT, NULL);
    playerInit ();
    fplayer->tl->x = 10;
    fplayer->tl->z = 90;
    fplayer->target = fighter [7];
    alliedInit (FIGHTER_HAWK2, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = 20;
    fighter [1]->tl->z = 100;
    fighter [1]->target = fighter [8];
    for (i = 2; i <= 6; i ++)
    {
      fighter [i]->tl->x = 20 + i * 5;
      fighter [i]->tl->z = 10;
      if (i == 3 || i == 4 || i == 5)
      {
        fighter [i]->o = &model_flarak1;
        fighter [i]->newinit (FLARAK_AIR1, 0, 400);
        fighter [i]->target = fighter [0];
      }
      else
      {
        fighter [i]->o = &model_flak1;
        fighter [i]->newinit (FLAK_AIR1, 0, 400);
        fighter [i]->target = fighter [0];
      }
      fighter [i]->party = 1;
    }
    for (i = 7; i <= 10; i ++)
    {
      int off = 40;
      if (difficulty == 0) off = 55;
      else off = 25;
      fighter [i]->tl->x = i * 5 - 50;
      fighter [i]->tl->z = -i * 5 - off;
      fighter [i]->o = &model_tank2;
      fighter [i]->newinit (TANK_GROUND1, 0, 300);
      fighter [i]->target = fighter [i - 4];
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 10; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (!fighter [3]->active || !fighter [4]->active || !fighter [5]->active)
      return 2;
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionDogfight3 : public Mission
{
  public:

  MissionDogfight3 ()
  {
    id = MISSION_DOGFIGHT3;
    strcpy (name, "DOGFIGHT");
    strcpy (briefing, "ENEMY FIGHTERS HAVE BEEN SIGHTED. YOU HAVE TO INTERCEPT THEM IMMEDIATELY. EXPECT BOMBERS WITHIN THEIR WING.");
    autoLFBriefing ();
    alliedfighters = 4;
    maxtime = 3000;
    selfighter [1] = FIGHTER_HAWK2;
    alliedpilot [0] = PILOT_PRIMETIME;
    alliedpilot [1] = PILOT_SHADOW;
    alliedpilot [2] = PILOT_LARA;
  }

  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 20;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_DESERT, NULL);
    playerInit ();
    fplayer->tl->x = 0;
    fplayer->tl->z = 100;
    alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = 5;
    fighter [1]->tl->z = 105;
    alliedInit (FIGHTER_FALCON, alliedpilot [1], fighter [2]);
    fighter [2]->tl->x = 10;
    fighter [2]->tl->z = 110;
    alliedInit (FIGHTER_FALCON, alliedpilot [2], fighter [3]);
    fighter [3]->tl->x = 15;
    fighter [3]->tl->z = 115;
    for (i = 4; i <= 8; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (4)];
      fighter [i]->o = &model_fige;
      fighter [i]->tl->x = -i * 10;
      fighter [i]->tl->z = -i * 10;
      fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 25);
    }
    for (i = 9; i <= 11; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (4)];
      fighter [i]->o = &model_figa;
      fighter [i]->tl->x = -i * 10;
      fighter [i]->tl->z = -i * 10;
      fighter [i]->newinit (FIGHTER_SWALLOW, 0, 400 - i * 25);
      fighter [i]->deactivate ();
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 15; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
    if (timer == 800)
    {
      fighter [9]->activate ();
      fighter [10]->activate ();
      fighter [11]->activate ();
      fighter [9]->tl->x = fplayer->tl->x + 55;
      fighter [9]->tl->z = fplayer->tl->z + 55;
      fighter [9]->tl->y = l->getHeight (fighter [9]->tl->x, fighter [9]->tl->z) + 10;
      fighter [10]->tl->x = fplayer->tl->x + 60;
      fighter [10]->tl->z = fplayer->tl->z + 60;
      fighter [10]->tl->y = l->getHeight (fighter [10]->tl->x, fighter [10]->tl->z) + 10;
      fighter [11]->tl->x = fplayer->tl->x + 65;
      fighter [11]->tl->z = fplayer->tl->z + 65;
      fighter [11]->tl->y = l->getHeight (fighter [11]->tl->x, fighter [11]->tl->z) + 10;
    }
    if (timer >= 800 && timer <= 1000)
      font1->drawTextCentered (0, 7, -3, "BOMBERS ARE ENTERING THE REGION", &textcolor);
  }
};

class MissionTank1 : public Mission
{
  public:

  MissionTank1 ()
  {
    id = MISSION_TANK1;
    strcpy (name, "TANKS");
    strcpy (briefing, "THE ENEMY IS GATHERING LOTS OF TANKS. FLY AN ASSAUT AND DESTROY THEM.");
    autoLFBriefing ();
    alliedfighters = 2;
    maxtime = 3000;
    selfighter [1] = FIGHTER_HAWK2;
    alliedpilot [0] = PILOT_SHADOW;
  }

  virtual void start ()
  {
    int i;
    day = 0;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 40;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_DESERT, NULL);
    playerInit ();
    fplayer->tl->x = 0;
    fplayer->tl->z = 100;
    alliedInit (FIGHTER_HAWK2, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = 5;
    fighter [1]->tl->z = 105;
    for (i = 2; i <= 6; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (2)];
      fighter [i]->o = &model_tank2;
      fighter [i]->tl->x = -i * 4;
      fighter [i]->tl->z = -i * 4;
      fighter [i]->newinit (TANK_GROUND1, 0, 400 - i * 25);
      fighter [i]->maxspeed = 0;
      fighter [i]->speed = 0;
    }
    for (i = 7; i <= 8; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (2)];
      fighter [i]->o = &model_tank1;
      fighter [i]->tl->x = i * 3;
      fighter [i]->tl->z = i * 3;
      fighter [i]->newinit (TANK_AIR1, 0, 50);
    }
    for (i = 9; i <= 10; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [0];
      fighter [i]->o = &model_container1;
      fighter [i]->tl->x = i * 3;
      fighter [i]->tl->z = 60;
      fighter [i]->newinit (STATIC_CONTAINER1, 0, 0);
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 15; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionShip1 : public Mission
{
  public:

  MissionShip1 ()
  {
    id = MISSION_SHIP1;
    strcpy (name, "DESTROYER");
    strcpy (briefing, "OUR SQUAD HAS BEEN ORDERED INTO THE MEDITERRANEAN. YOUR FIRST GOAL ARE SOME DESTROYERS, GUARDED BY CROWS.");
    autoLFBriefing ();
    alliedfighters = 2;
    maxtime = 3000;
    selfighter [1] = FIGHTER_HAWK2;
    alliedpilot [0] = PILOT_SHADOW;
  }

  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 50;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_SEA, NULL);
    playerInit ();
    fplayer->tl->x = 80;
    fplayer->tl->z = 0;
    fplayer->phi = 90;
    alliedInit (FIGHTER_HAWK2, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = 85;
    fighter [1]->tl->z = 5;
    fighter [1]->phi = 90;
    fighter [1]->target = fighter [6];
    for (i = 2; i <= 3; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (2)];
      fighter [i]->o = &model_ship2;
      fighter [i]->tl->x = -i * 4;
      fighter [i]->tl->z = -i * 4;
      fighter [i]->newinit (SHIP_DESTROYER1, 0, 50);
      fighter [i]->maxspeed = 0;
      fighter [i]->speed = 0;
    }
    for (i = 4; i <= 8; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (2)];
      fighter [i]->o = &model_fige;
      fighter [i]->tl->x = -i * 10;
      fighter [i]->tl->z = 0;
      fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 20);
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 15; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionShip2 : public Mission
{
  public:

  MissionShip2 ()
  {
    id = MISSION_SHIP2;
    strcpy (name, "OILRIG");
    strcpy (briefing, "AN OILRIG IS BEING ATTACKED BY ENEMY FORCES. DEFEND! OUR NEW SUPERIOR FIGHTER GL-15 IS NOW AVAILABLE.");
    autoLFBriefing ();
    alliedfighters = 2;
    maxtime = 2000;
    selfighter [0] = FIGHTER_REDARROW;
    selfighter [1] = FIGHTER_HAWK2;
    alliedpilot [0] = PILOT_SHADOW;
  }

  virtual void start ()
  {
    int i;
    day = 0;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 15;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_SEA, NULL);
    playerInit ();
    fplayer->tl->x = 80;
    fplayer->tl->z = 0;
    fplayer->phi = 90;
    alliedInit (FIGHTER_REDARROW, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = 85;
    fighter [1]->tl->z = 5;
    fighter [1]->phi = 90;
    fighter [1]->target = fighter [6];
    fighter [2]->o = &model_oilrig;
    fighter [2]->tl->x = 20;
    fighter [2]->tl->z = 0;
    fighter [2]->newinit (STATIC_OILRIG1, 0, 0);
    fighter [2]->maxspeed = 0;
    fighter [2]->speed = 0;
    fighter [2]->party = 1;
    for (i = 3; i <= 5; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (2)];
      fighter [i]->o = &model_fige;
      fighter [i]->tl->x = -50 - i * 10;
      fighter [i]->tl->z = 0;
      fighter [i]->newinit (FIGHTER_CROW, 0, 300 - i * 10);
    }
    for (i = 6; i <= 7; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [2];
      fighter [i]->o = &model_figa;
      fighter [i]->tl->x = -80 - i * 10;
      fighter [i]->tl->z = 0;
      fighter [i]->newinit (FIGHTER_SWALLOW, 0, 150);
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 15; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (!fighter [2]->active)
      return 2;
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionShip3 : public Mission
{
  public:

  MissionShip3 ()
  {
    id = MISSION_SHIP3;
    strcpy (name, "CRUISER ATTACK");
    strcpy (briefing, "THE CRUISER C-15 ARAKON HAS ENTERED NEUTRAL TERRITORY WATERS AND MUST BE ELIMINATED.");
    autoLFBriefing ();
    alliedfighters = 2;
    maxtime = 2000;
    selfighter [0] = FIGHTER_REDARROW;
    selfighter [1] = FIGHTER_HAWK2;
    alliedpilot [0] = PILOT_SHADOW;
  }

  virtual void start ()
  {
    int i;
    day = 0;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 50;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_SEA, NULL);
    playerInit ();
    fplayer->tl->x = 80;
    fplayer->tl->z = 0;
    fplayer->phi = 90;
    alliedInit (FIGHTER_HAWK2, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = 85;
    fighter [1]->tl->z = 5;
    fighter [1]->phi = 90;
    fighter [1]->target = fighter [6];
    fighter [2]->party = 0;
    fighter [2]->target = fighter [0];
    fighter [2]->o = &model_ship1;
    fighter [2]->tl->x = -20;
    fighter [2]->tl->z = 0;
    fighter [2]->newinit (SHIP_CRUISER, 0, 200);
    fighter [2]->maxspeed = 0;
    fighter [2]->speed = 0;
    for (i = 4; i <= 7; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (2)];
      fighter [i]->o = &model_fige;
      fighter [i]->tl->x = -i * 5;
      fighter [i]->tl->z = 0;
      fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 10);
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 15; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionCanyon1 : public Mission
{
  public:

  MissionCanyon1 ()
  {
    selfighter [0] = FIGHTER_REDARROW;
    selfighter [1] = FIGHTER_HAWK2;
    alliedpilot [0] = PILOT_SHADOW;
    id = MISSION_CANYON1;
    strcpy (name, "RADAR");
    strcpy (briefing, "YOU HAVE TO FLY AN ASSAULT ON A RADAR BASE. STAY AS LOW AS POSSIBLE!");
    autoLFBriefing ();
    alliedfighters = 1;
    maxtime = 2000;
  }

  virtual void start ()
  {
    int i, px, py;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 15;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_CANYON, NULL);
    l->searchPlain (-1, -1, &px, &py);
    px = px - MAXX / 2;
    py = MAXX / 2 - py;
    playerInit ();
    fplayer->tl->x = px + 130;
    fplayer->tl->z = py + 130;
    fplayer->phi = 45;
    fplayer->target = fighter [1];
    fighter [1]->o = &model_egg;
    fighter [1]->newinit (STATIC_COMPLEX1, 0, 0);
    fighter [1]->tl->x = px + 1;
    fighter [1]->tl->z = py + 1;
    fighter [1]->maxspeed = 0;
    fighter [1]->speed = 0;
    for (i = 2; i <= 4; i ++)
    {
      fighter [i]->o = &model_radar;
      fighter [i]->newinit (STATIC_RADAR1, 0, 0);
      fighter [i]->tl->x = px - 2 - (i - 2) * 2;
      fighter [i]->tl->z = py - 2 - (i - 2) * 2;
      fighter [i]->maxspeed = 0;
      fighter [i]->speed = 0;
    }
    for (i = 4; i <= 10; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [0];
      fighter [i]->o = &model_figd;
      fighter [i]->phi = 180;
      fighter [i]->newinit (FIGHTER_BUZZARD, 0, 50 + i * 20);
      fighter [i]->tl->x = px - 15 - i * 3;
      fighter [i]->tl->z = py - 15 - i * 3;
      fighter [i]->deactivate ();
    }
    for (i = 11; i <= 12; i ++)
    {
      fighter [i]->o = &model_flarak1;
      fighter [i]->target = fighter [0];
      fighter [i]->newinit (FLARAK_AIR1, 0, 100);
      fighter [i]->tl->x = px + 4;
      fighter [i]->tl->z = py + i * 3 - 30;
      fighter [i]->maxspeed = 0;
      fighter [i]->speed = 0;
      fighter [i]->phi = 220;
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    if (fplayer->tl->y >= fighter [1]->tl->y && dist (fplayer->tl->x - fighter [1]->tl->x, fplayer->tl->z - fighter [1]->tl->z) < 150)
    {
      for (i = 4; i <= 10; i ++)
      {
        if (!fighter [i]->active && fighter [i]->shield > 10)
          fighter [i]->activate ();
      }
    }
    for (i = 0; i <= 3; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionCanyon2 : public Mission
{
  public:

  MissionCanyon2 ()
  {
    selfighter [0] = FIGHTER_REDARROW;
    selfighter [1] = FIGHTER_HAWK2;
    alliedpilot [0] = PILOT_MATRIX;
    alliedpilot [1] = PILOT_PRIMETIME;
    alliedpilot [2] = PILOT_FIREBIRD;
    alliedpilot [3] = PILOT_LARA;
    id = MISSION_CANYON2;
    strcpy (name, "CANYON BATTLE");
    strcpy (briefing, "THIS MAY BECOME A REAL AIR BATTLE...");
    autoLFBriefing ();
    alliedfighters = 5;
    maxtime = 4000;
  }

  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 40;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_CANYON, NULL);
    playerInit ();
    fplayer->tl->x = 100;
    fplayer->tl->z = 100;
    fplayer->phi = 45;
    fplayer->target = fighter [5];
    for (i = 1; i <= 4; i ++)
    {
      alliedInit (FIGHTER_REDARROW, alliedpilot [i - 1], fighter [i]);
      if (i % 2)
        fighter [i]->tl->x = 100 + ((i + 1) / 2) * 5;
      else
        fighter [i]->tl->x = 100 - ((i + 1) / 2) * 5;
      fighter [i]->tl->z = 100 + i * 5;
      fighter [i]->target = fighter [5 + i];
      fighter [i]->phi = 45;
    }
    for (i = 5; i <= 22; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (5)];
      if (i <= 15)
      {
        fighter [i]->tl->x = -i * 5;
        fighter [i]->tl->z = -i * 5;
        fighter [i]->o = &model_fige;
        fighter [i]->newinit (FIGHTER_CROW, 0, myrandom (120) + 250);
      }
      else if (i <= 19)
      {
        fighter [i]->tl->x = -i * 8 - 160;
        fighter [i]->tl->z = -i * 8 - 160;
        fighter [i]->o = &model_figd;
        fighter [i]->newinit (FIGHTER_BUZZARD, 0, myrandom (120) + 150);
      }
      else
      {
        fighter [i]->tl->x = -i * 8 - 240;
        fighter [i]->tl->z = -i * 8 - 240;
        fighter [i]->o = &model_figa;
        fighter [i]->newinit (FIGHTER_SWALLOW, 0, myrandom (120) + 200);
      }
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 22; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionCanyon3 : public Mission
{
  public:

  MissionCanyon3 ()
  {
    selfighter [0] = FIGHTER_REDARROW;
    selfighter [1] = FIGHTER_HAWK2;
    alliedpilot [0] = PILOT_MATRIX;
    alliedpilot [1] = PILOT_PRIMETIME;
    alliedpilot [2] = PILOT_FIREBIRD;
    alliedpilot [3] = PILOT_LARA;
    id = MISSION_CANYON3;
    strcpy (name, "MAIN BASE");
    strcpy (briefing, "THERE IS ONE MAIN BASE IN THE CANYON, CONNECTED TO A BASE ON THE MOON. WE MUST TAKE OUT THIS BASE FIRST!");
    autoLFBriefing ();
    alliedfighters = 4;
    maxtime = 2500;
  }

  virtual void start ()
  {
    int i, px, py;
    day = 0;
    weather = WEATHER_THUNDERSTORM;
    camera = 0;
    sungamma = 45;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_CANYON, NULL);
    l->searchPlain (-1, -1, &px, &py);
    px = px - MAXX / 2;
    py = MAXX / 2 - py;
    playerInit ();
    fplayer->tl->x = px + 100;
    fplayer->tl->z = py + 100;
    fplayer->phi = 45;
    fplayer->target = fighter [5];
    for (i = 1; i <= 4; i ++)
    {
      alliedInit (FIGHTER_REDARROW, alliedpilot [i - 1], fighter [i]);
      if (i % 2)
        fighter [i]->tl->x = px + 100 + ((i + 1) / 2) * 5;
      else
        fighter [i]->tl->x = px + 100 - ((i + 1) / 2) * 5;
      fighter [i]->tl->z = py + 100 + i * 5;
      fighter [i]->target = fighter [5 + i];
      fighter [i]->phi = 45;
    }
    fighter [5]->o = &model_egg;
    fighter [5]->newinit (STATIC_COMPLEX1, 0, 0);
    fighter [5]->tl->x = px;
    fighter [5]->tl->z = py;
    fighter [5]->maxspeed = 0;
    fighter [5]->speed = 0;
    for (i = 6; i <= 9; i ++)
    {
      fighter [i]->o = &model_hall1;
      fighter [i]->newinit (STATIC_HALL1, 0, 100);
      fighter [i]->maxspeed = 0;
      fighter [i]->speed = 0;
    }
    fighter [6]->tl->x = px + 3;
    fighter [6]->tl->z = py + 3;
    fighter [7]->tl->x = px - 3;
    fighter [7]->tl->z = py + 3;
    fighter [8]->tl->x = px - 3;
    fighter [8]->tl->z = py - 3;
    fighter [9]->tl->x = px + 3;
    fighter [9]->tl->z = py - 3;
    for (i = 10; i <= 22; i ++)
    {
      fighter [i]->party = 0;
      fighter [i]->target = fighter [myrandom (5)];
      fighter [i]->phi = 180;
      if (i <= 16)
      {
        fighter [i]->o = &model_figb;
        fighter [i]->newinit (FIGHTER_CROW, 0, i * 10);
      }
      else
      {
        fighter [i]->o = &model_figd;
        fighter [i]->newinit (FIGHTER_BUZZARD, 0, i * 10);
      }
      fighter [i]->tl->x = px - i * 3;
      fighter [i]->tl->z = py - i * 3;
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 9; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionMoonDefense1 : public Mission
{
  public:

  MissionMoonDefense1 ()
  {
    selfighter [0] = FIGHTER_REDARROW;
    selfighter [1] = FIGHTER_PHOENIX;
    alliedpilot [0] = PILOT_LARA;
    id = MISSION_MOON1;
    strcpy (name, "TURRETS");
    strcpy (briefing, "THE DESTINY OF OUR FUTURE IS SOMEWHERE ON THE MOON. OUR NEW BOMBER GL-117 WITH ITS SUPERIOR SHIELDS SHOULD BE EXTREMELY HELPFUL. AT FIRST, WE HAVE TO TAKE OUT THE TURRETS.");
    autoLFBriefing ();
    alliedfighters = 2;
    maxtime = 4000;
  }

  virtual void start ()
  {
    int i;
    day = 0;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 50;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_MOON, NULL);
    playerInit ();
    fplayer->tl->x = 100;
    fplayer->tl->z = 100;
    fplayer->phi = 45;
    fplayer->target = fighter [6];
    alliedInit (FIGHTER_PHOENIX, alliedpilot [0], fighter [1]);
    fighter [1]->tl->x = 105;
    fighter [1]->tl->z = 105;
    fighter [1]->target = fighter [6];
    fighter [1]->phi = 45;
    for (i = 2; i <= 10; i ++)
    {
      int ix = (i - 2) % 3;
      int iy = (i - 2) / 3;
      fighter [i]->tl->x = ix * 10;
      fighter [i]->tl->z = iy * 10;
      fighter [i]->target = fighter [myrandom (2)];
      fighter [i]->o = &model_flak1;
      fighter [i]->newinit (FLAK_AIR1, 0, 200);
    }
    for (i = 11; i <= 20; i ++)
    {
      int ix = (i - 11) % 3;
      int iy = (i - 11) / 3;
      fighter [i]->tl->x = ix * 20 + 40;
      fighter [i]->tl->z = iy * 20 + 40;
      fighter [i]->target = fighter [myrandom (2)];
      fighter [i]->o = &model_mine1;
      fighter [i]->newinit (MISSILE_MINE1, 0, 220);
    }
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 10; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionMoonDogfight1 : public Mission
{
  public:

  MissionMoonDogfight1 ()
  {
    selfighter [0] = FIGHTER_REDARROW;
    selfighter [1] = FIGHTER_PHOENIX;
    alliedpilot [0] = PILOT_MATRIX;
    id = MISSION_MOON2;
    strcpy (name, "ELITE DOGFIGHT");
    strcpy (briefing, "ENEMY FIGHTERS HAVE APPEARED IN THIS REGION.");
    autoLFBriefing ();
    alliedfighters = 2;
    maxtime = 2500;
  }

  virtual void start ()
  {
    day = 0;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 50;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_MOON, NULL);
    playerInit ();
    fplayer->tl->x = -100;
    fplayer->tl->z = -100;
    fplayer->phi = 200;
    fplayer->target = fighter [2];
    fighter [1]->tl->x = -110;
    fighter [1]->tl->z = -110;
    fighter [1]->phi = 200;
    fighter [1]->target = fighter [3];
    alliedInit (FIGHTER_REDARROW, alliedpilot [0], fighter [1]);
    fighter [2]->tl->x = 0;
    fighter [2]->tl->z = 0;
    fighter [2]->target = fighter [0];
    fighter [2]->phi = 200;
    fighter [2]->o = &model_figh;
    fighter [2]->newinit (FIGHTER_BLACKBIRD, 0, 150);
    fighter [3]->tl->x = 10;
    fighter [3]->tl->z = 10;
    fighter [3]->target = fighter [1];
    fighter [3]->phi = 200;
    fighter [3]->o = &model_figh;
    fighter [3]->newinit (FIGHTER_BLACKBIRD, 0, 200);
    fighter [4]->tl->x = 200;
    fighter [4]->tl->z = 200;
    fighter [4]->target = fighter [0];
    fighter [4]->phi = 200;
    fighter [4]->o = &model_figh;
    fighter [4]->newinit (FIGHTER_BLACKBIRD, 0, 40);
    fighter [5]->tl->x = 210;
    fighter [5]->tl->z = 210;
    fighter [5]->target = fighter [1];
    fighter [5]->phi = 200;
    fighter [5]->o = &model_figh;
    fighter [5]->newinit (FIGHTER_BLACKBIRD, 0, 140);
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 15; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionMoonBase1 : public Mission
{
  public:

  MissionMoonBase1 ()
  {
    selfighter [0] = FIGHTER_REDARROW;
    selfighter [1] = FIGHTER_PHOENIX;
    id = MISSION_MOON3;
    strcpy (name, "SNEAKING");
    strcpy (briefing, "TIME TO ATTACK. THE BASE IS HEAVILY GUARDED BY BLACKBIRDS. USE AN ASTEROID FIELD TO GET NEAR THE BASE AND DESTROY IT.");
    autoLFBriefing ();
    alliedfighters = 1;
    maxtime = 3500;
  }

  virtual void start ()
  {
    int i;
    day = 0;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 50;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_FLAT_MOON, NULL);
    playerInit ();
    fplayer->tl->x = -20;
    fplayer->tl->z = -40;
    fplayer->phi = 200;
    fplayer->target = fighter [29];
    for (i = 1; i < 21; i ++)
    {
      fighter [i]->tl->x = cosi [(i * 140) % 360] * 4.5;
      fighter [i]->tl->z = i * 0.6;
      fighter [i]->tl->y = l->getHeight (fighter [i]->tl->x, fighter [i]->tl->z) + sine [(i * 160) % 360] * 4.5 + 25.0;
      fighter [i]->target = fighter [0];
      fighter [i]->phi = 200;
      fighter [i]->o = &model_aster1;
      fighter [i]->newinit (ASTEROID, 0, 150);
      fighter [i]->party = 0;
    }
    for (i = 21; i < 29; i ++)
    {
      fighter [i]->tl->x = (i - 20) * 4;
      fighter [i]->tl->z = 210;
      fighter [i]->target = fighter [0];
      fighter [i]->phi = 50;
      fighter [i]->o = &model_figh;
      fighter [i]->newinit (FIGHTER_BLACKBIRD, 0, 100);
    }
    fighter [29]->tl->x = 50 + difficulty * 15;
    fighter [29]->tl->z = 180 + difficulty * 30;
    fighter [29]->o = &model_base1;
    fighter [29]->newinit (STATIC_BASE1, 0, 100);
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 29; i <= 29; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionMoon1 : public Mission
{
  public:

  MissionMoon1 ()
  {
    id = MISSION_MOON;
    strcpy (name, "MOON");
    strcpy (briefing, "THIS IS JUST A SIMPLE DEMO MISSION");
    autoLFBriefing ();
    alliedfighters = 1;
    maxtime = 2000;
  }

  virtual void start ()
  {
    day = 0;
    weather = WEATHER_CLOUDY;
    camera = 0;
    sungamma = 50;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_MOON, NULL);
    playerInit ();
    fplayer->tl->x = 50;
    fplayer->tl->z = 50;
    fighter [1]->target = fighter [0];
    fighter [1]->o = &model_figa;
    fighter [1]->newinit (FIGHTER_SWALLOW, 0, 200);
    fighter [2]->target = fighter [0];
    fighter [2]->o = &model_figa;
    fighter [2]->newinit (FIGHTER_SWALLOW, 0, 150);
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 4; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 4, -2, name, &textcolor);
    }
  }
};

class MissionMultiDogfight1 : public Mission
{
  public:

  MissionMultiDogfight1 ()
  {
    id = MISSION_MULTIPLAYER_DOGFIGHT;
    strcpy (name, "MULTIPLAYER");
    strcpy (briefing, "THIS IS JUST A SIMPLE DEMO MISSION");
    autoLFBriefing ();
    alliedfighters = 2;
  }

  virtual void start ()
  {
    int i;
    day = 1;
    weather = WEATHER_SUNNY;
    camera = 0;
    sungamma = 25;
    if (l != NULL) delete l;
    l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
    for (i = 0; i <= 1; i ++)
    {
      fighter [i]->activate ();
      fighter [i]->ai = false;
      fighter [i]->party = i;
      fighter [i]->target = fighter [0];
      fighter [i]->o = &model_fig;
      fighter [i]->tl->x = 50 + i * 30;
      fighter [i]->tl->z = 100;
    }
    fighter[1]->ai = true;
    if (isserver)
      fplayer = fighter [0];
    else
      fplayer = fighter [1];
    if (controls != 100)
      fplayer->easymodel = false;
  }

  virtual int processtimer ()
  {
    bool b = false;
    int i;
    timer ++;
    if (!fplayer->active && fplayer->explode >= 35)
    {
      return 2;
    }
    for (i = 0; i <= 4; i ++)
    {
      if (fighter [i]->active)
        if (fighter [i]->party == 0)
          b = true;
    }
    if (b) return 0;
    return 1;
  }

  virtual void draw ()
  {
    if (timer >= 0 && timer <= 50)
    {
      font1->drawTextCentered (0, 0, -1, "DOGFIGHT");
    }
  }
};

Mission *mission = NULL;
Mission *missionnew = NULL;

class Cockpit
{
  public:

/*  float getGamma (DynamicObj *obj)
  {
    float dy = obj->tl->y - fplayer->tl->y;
    float dx = fplayer->distance (obj);
    if (dx == 0) dx = 1E-5;
    return atan (dy / dx) * 180.0 / PI;
  }

  float getPhi (DynamicObj *obj)
  {
    float dx2 = obj->tl->x - fplayer->tl->x, dz2 = obj->tl->z - fplayer->tl->z; // actual distances
    float a, aw, w = fplayer->phi;
    if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;
    a = atan (dx2 / dz2) * 180 / PI;
    if (dz2 > 0)
    {
      if (dx2 > 0) a -= 180;
      else a += 180;
    }
    aw = a - w;
    if (aw < -180) aw += 360;
    if (aw > 180) aw -= 360;
    return aw;
  }*/

  void drawTargeter ()
  {
    if (fplayer->target)
    if (fplayer->target->active)
    {
/*      float gamma = getGamma (fplayer->target);
      float phi = getPhi (fplayer->target);
      float mx = width / 2, my = height / 2;
      float x = mx + phi / 90.0 * width;
      float y = my + gamma / 90.0 * height;
      float dx = x - mx, dy = my - y;
      int t = (int) fplayer->theta;
      if (t < 0) t += 360;
      float rx = dx * cosi [t] - dy * sine [t];
      float ry = dx * sine [t] + dy * cosi [t];
      rx += mx;
      ry = my - ry;*/
/*      DynamicObj *o = fplayer->target;
      glBegin (GL_LINE_STRIP);
      glColor4ub (255, 255, 0, 150);
      glVertex3f (o->tl->x - o->zoom, o->tl->y - o->zoom, o->tl->z - o->zoom);
      glVertex3f (o->tl->x + o->zoom, o->tl->y - o->zoom, o->tl->z - o->zoom);
      glVertex3f (o->tl->x + o->zoom, o->tl->y - o->zoom, o->tl->z + o->zoom);
      glVertex3f (o->tl->x - o->zoom, o->tl->y - o->zoom, o->tl->z + o->zoom);
      glVertex3f (o->tl->x - o->zoom, o->tl->y - o->zoom, o->tl->z - o->zoom);
      glVertex3f (o->tl->x - o->zoom, o->tl->y + o->zoom, o->tl->z - o->zoom);
      glVertex3f (o->tl->x + o->zoom, o->tl->y + o->zoom, o->tl->z - o->zoom);
      glVertex3f (o->tl->x + o->zoom, o->tl->y + o->zoom, o->tl->z + o->zoom);
      glVertex3f (o->tl->x - o->zoom, o->tl->y + o->zoom, o->tl->z + o->zoom);
      glVertex3f (o->tl->x - o->zoom, o->tl->y + o->zoom, o->tl->z - o->zoom);
      glEnd ();
      glBegin (GL_LINES);
      glVertex3f (o->tl->x + o->zoom, o->tl->y - o->zoom, o->tl->z - o->zoom);
      glVertex3f (o->tl->x + o->zoom, o->tl->y + o->zoom, o->tl->z - o->zoom);
      glVertex3f (o->tl->x + o->zoom, o->tl->y - o->zoom, o->tl->z + o->zoom);
      glVertex3f (o->tl->x + o->zoom, o->tl->y + o->zoom, o->tl->z + o->zoom);
      glVertex3f (o->tl->x - o->zoom, o->tl->y - o->zoom, o->tl->z + o->zoom);
      glVertex3f (o->tl->x - o->zoom, o->tl->y + o->zoom, o->tl->z + o->zoom);
      glEnd ();*/
      DynamicObj *o = fplayer->target;
      float myphi = fplayer->phi;
      if (myphi < 0) myphi += 360;
      float ex1 = cosi [(int) myphi] * o->zoom, ey1 = -sine [(int) myphi] * o->zoom;
      float ex2 = -ex1, ey2 = -ey1;
      float ez = o->zoom;
      gl->enableAlphaBlending ();
      glBegin (GL_LINE_STRIP);
      if (((AIObj *) fplayer->target)->party != fplayer->party)
        glColor4ub (255, 0, 0, 255);
      else
        glColor4ub (0, 0, 255, 255);
      glVertex3f (o->tl->x + ex1, o->tl->y + ez, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex2, o->tl->y + ez, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex2, o->tl->y - ez, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex1, o->tl->y - ez, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex1, o->tl->y + ez, o->tl->z + ey1);
      glEnd ();
      gl->disableAlphaBlending ();
    }

  }

  void drawCross ()
  {
    glDisable (GL_LIGHTING);
/*    gl->enableAlphaBlending ();
    glDisable (GL_DEPTH_TEST);
    glLineWidth (2.0);
    glDisable (GL_DEPTH_TEST);
    glBegin (GL_LINES);
    glColor4f (1.0, 1.0, 0.0, 0.5);
    float zf = -18.0, g = 2.0, g2 = 0.8;
    glVertex3f (-g, 0.0, zf);
    glVertex3f (g, 0.0, zf);
    glVertex3f (0.0, -g, zf);
    glVertex3f (0.0, g, zf);
    glVertex3f (-g2, -g, zf);
    glVertex3f (g2, -g, zf);
    glVertex3f (-g2, g, zf);
    glVertex3f (g2, g, zf);
    glVertex3f (-g, -g2, zf);
    glVertex3f (-g, g2, zf);
    glVertex3f (g, -g2, zf);
    glVertex3f (g, g2, zf);
    glEnd ();
    gl->disableAlphaBlending ();*/

  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
//  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  gl->enableTextures (texcross->textureID);

  float xf = 0.1, yf = 0.1, zf = 1.0;
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 0, 255);
  glTexCoord2d (0, 0);
  glVertex3f (-xf, -yf, -zf);
  glTexCoord2d (0, 1);
  glVertex3f (-xf, yf, -zf);
  glTexCoord2d (1, 1);
  glVertex3f (xf, yf, -zf);
  glTexCoord2d (1, 0);
  glVertex3f (xf, -yf, -zf);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_TEXTURE_2D);
  gl->disableAlphaBlending ();
//  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }

  void drawHeading ()
  {
    CColor color;
    int i = 0;
    int alpha = 175;
    color.setColor (255, 255, 0);
    color.c [3] = alpha;
    glDisable (GL_LIGHTING);
    glLineWidth (2.0);
    glDisable (GL_DEPTH_TEST);
    float xf;
    float yf = 17.5;
    float zf = -2.5;
    float g = 0.5;
//    float p1 = fplayer->phi - 40.0;
//    float p2 = fplayer->phi + 40.0;
    for (i = 0; i < 360; i += 5)
    {
      float p = (float) i - fplayer->phi;
      if (p < -180) p += 360;
      if (p > 180) p -= 360;
      if (p >= -40 && p <= 40)
      {
        if (i == 0 || i == 90 || i == 180 || i == 270)
          g = 0.6;
        else if (!(i % 10))
          g = 0.3;
        else
          g = 0.1;
        xf = p / 6.0;
        gl->enableAlphaBlending ();
        glBegin (GL_LINES);
        glColor4ub (255, 255, 0, alpha);
        glVertex3f (xf * 0.1, (yf - g) * 0.1, zf);
        glVertex3f (xf * 0.1, (yf + g) * 0.1, zf);
        glEnd ();
        if (i == 0)
          font1->drawText (xf - 0.5, yf - 2.0, zf, "N", &color);
        else if (i == 90)
          font1->drawText (xf - 0.5, yf - 2.0, zf, "W", &color);
        else if (i == 180)
          font1->drawText (xf - 0.5, yf - 2.0, zf, "S", &color);
        else if (i == 270)
          font1->drawText (xf - 0.5, yf - 2.0, zf, "E", &color);
      }
    }

/*    if (fplayer->target != NULL)
      if (fplayer->target->active)
      {
        int aw = fplayer->getAngle (fplayer->target);
//        printf ("aw = %d\n", aw);
        if (aw < -50)
        {
          glBegin (GL_LINE_STRIP);
          if (((AIObj *) fplayer->target)->party != fplayer->party)
            glColor4ub (255, 0, 0, alpha);
          else
            glColor4ub (0, 0, 255, alpha);
          glVertex3f (0.6, 0.1 * (yf - 1), zf);
          glVertex3f (0.65, 0.1 * yf, zf);
          glVertex3f (0.6, 0.1 * (yf + 1), zf);
          glEnd ();
        }
        else if (aw > 50)
        {
          glBegin (GL_LINE_STRIP);
          if (((AIObj *) fplayer->target)->party != fplayer->party)
            glColor4ub (255, 0, 0, alpha);
          else
            glColor4ub (0, 0, 255, alpha);
          glVertex3f (-0.6, 0.1 * (yf - 1), zf);
          glVertex3f (-0.65, 0.1 * yf, zf);
          glVertex3f (-0.6, 0.1 * (yf + 1), zf);
          glEnd ();
        }
      }*/

    if (fplayer->target != NULL)
      if (fplayer->target->active)
      {
        float disttarget = fplayer->distance (fplayer->target);
        float dgamma = atan ((fplayer->target->tl->y - fplayer->tl->y) / disttarget) * 180 / PI - (fplayer->gamma - 180);
        float dphi = fplayer->getAngle (fplayer->target);
        if (fabs (dphi) > 50 || fabs (dgamma) > 50)
        {
          if (dphi < -90) dphi = -90;
          else if (dphi > 90) dphi = 90;
          float delta = atan (dgamma / dphi) * 180 / PI;
          if (dphi > 0) delta -= 180;
  //        printf ("delta=%f, dgamma=%f, dphi=%f\n",delta, dgamma, dphi);
          delta += fplayer->theta;
          while (delta < 0) delta += 360;
          while (delta >= 360) delta -= 360;
          float xs = cosi [(int) delta] * 3;
          float ys = -sine [(int) delta] * 3;
  /*        if (xs > 3) xs = 3;
          else if (xs < -3) xs = -3;
          if (ys > 3) ys = 3;
          else if (ys < -3) ys = -3;*/
          zf = -4;
          glBegin (GL_LINE_STRIP);
          glColor4ub (255, 0, 0, alpha);
          glVertex3f (xs - 0.12, ys - 0.12, zf);
          glVertex3f (xs + 0.12, ys - 0.12, zf);
          glVertex3f (xs + 0.12, ys + 0.12, zf);
          glVertex3f (xs - 0.12, ys + 0.12, zf);
          glVertex3f (xs - 0.12, ys - 0.12, zf);
          glEnd ();
          glBegin (GL_LINES);
          glVertex3f (xs - 0.15, ys - 0.15, zf);
          glVertex3f (xs + 0.15, ys + 0.15, zf);
          glVertex3f (xs + 0.15, ys - 0.15, zf);
          glVertex3f (xs - 0.15, ys + 0.15, zf);
          glEnd ();
        }
/*        else if (disttarget > view)
        {
          float xs = -sin (dphi * PI / 180.0) * 3;
          float zf = -4;
          float ys = sin (dgamma * PI / 180.0) * 3;
          glBegin (GL_LINE_STRIP);
          glColor4ub (255, 0, 0, alpha);
          glVertex3f (xs - 0.12, ys - 0.12, zf);
          glVertex3f (xs + 0.12, ys - 0.12, zf);
          glVertex3f (xs + 0.12, ys + 0.12, zf);
          glVertex3f (xs - 0.12, ys + 0.12, zf);
          glVertex3f (xs - 0.12, ys - 0.12, zf);
          glEnd ();
          glBegin (GL_LINES);
          glVertex3f (xs - 0.15, ys - 0.15, zf);
          glVertex3f (xs + 0.15, ys + 0.15, zf);
          glVertex3f (xs + 0.15, ys - 0.15, zf);
          glVertex3f (xs - 0.15, ys + 0.15, zf);
          glEnd ();
        }*/
      }

    zf = -3.5;
    for (i = 0; i < 360; i += 5)
    {
      float p = (float) i - fplayer->gamma;
      if (p < -180) p += 360;
      if (p > 180) p -= 360;
      if (p >= -30 && p <= 30)
      {
        if (i == 0 || i == 90 || i == 180 || i == 270)
          g = 0.6;
        else if (!(i % 10))
          g = 0.3;
        else
          g = 0.1;
        xf = p / 6.0;
        gl->enableAlphaBlending ();
        yf = -7;
        glBegin (GL_LINES);
        glColor4ub (255, 255, 0, alpha);
        glVertex3f ((yf - g) * 0.1, xf * 0.1, zf);
        glVertex3f ((yf + g) * 0.1, xf * 0.1, zf);
        glEnd ();
        yf = -8.5;
        if (i == 120)
          font1->drawText (yf - 2.0, xf - 0.5, zf, "-60", &color);
        else if (i == 150)
          font1->drawText (yf - 2.0, xf - 0.5, zf, "-30", &color);
        else if (i == 180)
          font1->drawText (yf - 2.0, xf - 0.5, zf, "0", &color);
        else if (i == 210)
          font1->drawText (yf - 2.0, xf - 0.5, zf, "30", &color);
        else if (i == 240)
          font1->drawText (yf - 2.0, xf - 0.5, zf, "60", &color);
      }
    }

    gl->enableAlphaBlending ();
    int t1 = (int) (fplayer->theta - 135.0);
    int t2 = t1 + 90, t3 = t1 + 180, t4 = t1 + 270;
    if (t1 != 0) t1 %= 360;
    if (t2 != 0) t2 %= 360;
    if (t3 != 0) t3 %= 360;
    if (t4 != 0) t4 %= 360;
    if (t1 < 0) t1 += 360;
    if (t2 < 0) t2 += 360;
    if (t3 < 0) t3 += 360;
    if (t4 < 0) t4 += 360;
    xf = 0.6; yf = 0.6;
    glBegin (GL_LINES);
    glColor4ub (255, 255, 0, 130);
    glVertex3f (xf * cosi [t1], yf * sine [t1], zf);
    glVertex3f (xf * cosi [t2], yf * sine [t2], zf);
    glVertex3f (xf * cosi [t3], yf * sine [t3], zf);
    glVertex3f (xf * cosi [t4], yf * sine [t4], zf);
    glEnd ();
    gl->disableAlphaBlending ();
  }

  void drawTargetedElement ()
  {
    char str [20];
    CColor color;
    CVector3 tl, n;
    CRotation r;
    color.setColor (255, 0, 0);
    color.c [3] = 255;
    tl.x = -0.35; tl.y = -0.3; tl.z = -0.5;
    gl->disableAlphaBlending ();
    glEnable (GL_DEPTH_TEST);
    if (fplayer->target != NULL)
      if (fplayer->target->active)
      {
        glEnable (GL_LIGHTING);
        fplayer->target->o->draw (&n, &tl, fplayer->target->rot, 0.05, 1.0, 0);
        glDisable (GL_LIGHTING);
        if (((AIObj *) fplayer->target)->party == fplayer->party)
          color.setColor (0, 0, 255);
        font1->drawText (-24.0, -23.0, -4.0, fplayer->target->o->name, &color);
        sprintf (str, "%d", (int) (15.0 * fplayer->distance (fplayer->target)));
        font1->drawText (-24.0, -25.0, -4.0, str, &color);
      }
/*    color.setColor (255, 255, 255);
    sprintf (str, "MISSILES: %d", fplayer->missiles);
    font1->drawText (20.0, -25.0, -4.0, str, &color);*/
    glDisable (GL_DEPTH_TEST);
//    font1->drawText (0, 0, -5.0, "ABCDEFG", &color);
  }

  void drawWeapon ()
  {
    char str [20];
    CColor color;
    CVector3 tl, n;
    CRotation r;
    if (fplayer->missiles [fplayer->missiletype] <= 0)
      return;
    color.setColor (255, 0, 0);
    color.c [3] = 255;
    tl.x = 0.35; tl.y = -0.3; tl.z = -0.5;
    gl->disableAlphaBlending ();
    glEnable (GL_DEPTH_TEST);
    CModel *missile;
    if (fplayer->missiletype == 0) missile = &model_missile1;
    else if (fplayer->missiletype == 1) missile = &model_missile2;
    else if (fplayer->missiletype == 2) missile = &model_missile3;
    else if (fplayer->missiletype == 3) missile = &model_missile4;
    else if (fplayer->missiletype == 4) missile = &model_missile5;
    else if (fplayer->missiletype == 5) missile = &model_missile6;
    glEnable (GL_LIGHTING);
    missile->draw (&n, &tl, fplayer->rot, 0.05, 1.0, 0);
    glDisable (GL_LIGHTING);
    glDisable (GL_DEPTH_TEST);
    font1->drawText (16.0, -22.0, -4.0, missile->name, &color);
    sprintf (str, "N %d", fplayer->missiles [fplayer->missiletype]);
    font1->drawText (16.0, -24.0, -4.0, str, &color);
  }

  void drawRadar ()
  {
    int i;
    float yf = -4.5, zf = -7.0;
    gl->enableAlphaBlending ();
    glDisable (GL_DEPTH_TEST);
    glBegin (GL_QUADS);
    glColor4ub (255, 255, 0, 100);
    glVertex3f (-1.2, yf - 1.0, zf);
    glVertex3f (1.2, yf - 1.0, zf);
    glVertex3f (1.2, yf + 1.0, zf);
    glVertex3f (-1.2, yf + 1.0, zf);
    glEnd ();
    glPointSize (2.0);
    glBegin (GL_POINTS);
    glColor4ub (200, 200, 200, 255);
    glVertex3f (0.0, yf, zf);
    for (i = 0; i < maxfighter; i ++)
      if (fighter [i] != fplayer && fighter [i]->active)
      {
        int aw = fplayer->getAngle (fighter [i]);
        if (aw < 0) aw += 360;
        float d = fplayer->distance (fighter [i]) / 100.0;
        float px = - d * sine [aw];
        float py = yf + d * cosi [aw];
        if (px >= -1.2 && px <= 1.2 && py >= yf - 1.0 && py <= yf + 1.0)
        {
          if (fighter [i] == fplayer->target)
            glColor4ub (255, 200, 0, 255);
          else if (fighter [i]->party != fplayer->party)
            glColor4ub (255, 0, 0, 255);
          else
            glColor4ub (0, 0, 255, 255);
          glVertex3f (px, py, zf);
        }
      }
    glEnd ();
    gl->disableAlphaBlending ();
  }

};




class ConfigFile
{
  public:
  char buf [32000];
  int length;
  FILE *out;
  ConfigFile () {}
  ConfigFile (char *fname)
  {
//    int z;
    bool commentmode = false;
    FILE *in;
    length = 0;
    in = fopen (fname, "rb");
    if (in != NULL)
    {
      length = fread (buf, 1, 32000, in);
      fclose (in);
    }
    else
    {
      fprintf (stderr, "\nCould not load %s.", fname);
      fflush (stderr);
      buf [0] = 0;
    }
    for (int i = 0; i < length; i ++)
    {
      if (buf [i] == '#') commentmode = true;
      if (buf [i] == '\n') commentmode = false;
      if (commentmode) buf [i] = ' ';
      else buf [i] = tolower (buf [i]);
    }
  }
  char *skipwhite (char *str)
  {
    while (*str == ' ' || *str == '\t')
      str ++;
    return str;
  }
  char *skipnum (char *str)
  {
    while (*str >= '0' && *str <= '9')
      str ++;
    return str;
  }
  char *getString (char *dest, char *str)
  {
    char *strf, *stre;
    char cmpstr [256];
    int i;
    if (strlen (str) >= 256) { return NULL; }
    strcpy (cmpstr, str);
    if (buf [0] == 0) { return NULL; }
    for (i = 0; i < (int) strlen (cmpstr); i ++)
    {
      cmpstr [i] = tolower (cmpstr [i]);
    }
    strf = strstr (buf, cmpstr);
    if (strf == NULL) { return NULL; }
    strf += strlen (cmpstr);
    strf = skipwhite (strf);
    if (*strf == 0) { return NULL; }
    strf ++;
    if (*strf == 0) { return NULL; }
    strf = skipwhite (strf);
    if (*strf == 0) { return NULL; }
    stre = skipnum (strf);
    if (stre - strf >= 256) { return NULL; }
    if (*strf == 0) { return NULL; }
    for (i = 0; i < (int) (stre - strf); i ++)
      dest [i] = *(strf+i);
    dest [i] = 0;
    return dest;
  }
  int getValue (char *str)
  {
    char res [256];
    getString (res, str);
    if (res == NULL) return -1;
    return atoi (res);
  }
  int openOutput (char *fname)
  {
    out = fopen (fname, "wb");
    if (out == NULL) return 0;
    return 1;
  }
  int write (char *str1, int n)
  {
    char str [256];
    if (strlen (str1) + 8 > 256) return 0;
    sprintf (str, "%s = %d\n", str1, n);
    fwrite (str, 1, strlen (str), out);
    return 1;
  }
  void writeText (char *str)
  {
    fwrite (str, 1, strlen (str), out);
    fwrite ("\n", 1, 1, out);
  }
  void close ()
  {
    fclose (out);
  }
};

bool firststart = false;

void save_config ()
{
  ConfigFile *cf = new ConfigFile ();
  char *confname = dirs->getSaves ("conf");
  printf ("\nSaving %s ", confname); fflush (stdout);
  int ret1 = cf->openOutput (confname);
  if (ret1 == 0)
  {
    fprintf (stderr, "\nCould not save configuration.");
    fflush (stderr);
    return;
  }
  cf->writeText ("# Configuration\n");
  cf->writeText ("# Some possible width x height values for fullscreen mode:");
  cf->writeText ("#  640x480, 800x600, 1024x768");
  cf->write (" width", width);
  cf->write (" height", height);
  cf->writeText ("# Possible bits per pixel values (color depth):");
  cf->writeText ("#  8 (not recommended), 16, 24, 32");
  cf->write (" bpp", bpp);
  cf->writeText ("# Try to go fullscreen = 1, game in window = 0");
  cf->write (" fullscreen", fullscreen);
  cf->writeText ("# Quality: 0=software rendered up to 5=best (default=3)");
  cf->write (" quality", quality);
  cf->writeText ("# Far clipping plane: 20..100 (default=50)");
  cf->write (" view", (int) view);
  cf->writeText ("# Dithering: 0=off, 1=on (default=0)");
  cf->write (" dithering", dithering);
#ifdef HAVE_SDL_MIXER
  cf->writeText ("# Sound volume: 0..100 (default=100) per cent");
  cf->write (" sound", (int) volumesound);
  cf->writeText ("# Music volume: 0..100 (default=100) per cent");
  cf->write (" music", (int) volumemusic);
#endif
  cf->writeText ("# Piloting controls: 0=keyboard, 1=mouse, 2=joystick");
  cf->write (" controls", controls);
  cf->writeText ("# Difficulty level: 0=easy, 1=medium, 2=hard");
  cf->write (" difficulty", difficulty);
/*  cf->writeText ("# Available display modes, do not alter these!");
  cf->write (" mode0", modes [0]);
  cf->write (" mode1", modes [1]);
  cf->write (" mode2", modes [2]);
  cf->write (" mode3", modes [3]);*/
  cf->writeText ("\n# This file is meant to give sensible startup settings");
  cf->writeText ("# as graphic cards and drivers may differ some 100 times in speed");
  cf->writeText ("\n# To get back to default settings, just delete this file!");
  cf->close ();
  delete cf;
}

int load_config ()
{
  char ret [256];
  char *str;
  char *confname = dirs->getSaves ("conf");
  printf ("\nLoading %s ", confname); fflush (stdout);
  ConfigFile *cf = new ConfigFile (confname);

  str = cf->getString (ret, "width");
  if (str == NULL)
  { width = 800; }
  else
  { width = atoi (str); }
  if (width < 100) width = 100;
  else if (width > 3000) width = 3000;

  str = cf->getString (ret, "height");
  if (str == NULL)
  { height = 600; }
  else
  { height = atoi (str); }
  if (height < 100) height = 100;
  else if (height > 2000) height = 2000;

  str = cf->getString (ret, "bpp");
  if (str == NULL)
  { bpp = 32; }
  else
  { bpp = atoi (str); }
  if (bpp != 8 && bpp != 16 && bpp != 24 && bpp != 32)
    bpp = 32;

  str = cf->getString (ret, "fullscreen");
  if (str == NULL)
  { fullscreen = 1; }
  else
  { fullscreen = atoi (str); }
  if (fullscreen) fullscreen = 1;

  str = cf->getString (ret, "quality");
  if (str == NULL)
  { quality = 2; }
  else
  { quality = atoi (str); }
  if (quality < 0) quality = 0;
  else if (quality > 5) quality = 5;

  str = cf->getString (ret, "view");
  if (str == NULL)
  { view = 50; }
  else
  { view = atoi (str); }
  if (view < 20)
  {
    view = 20;
//    firststart = true;
  }
  else if (view > 100)
  {
    view = 100;
  }

  str = cf->getString (ret, "dithering");
  dithering = (str == NULL)? 0 : atoi(str);
  if (dithering) dithering = 1;

#ifdef HAVE_SDL_MIXER
  str = cf->getString (ret, "sound");
  if (str == NULL)
  { volumesound = 100; }
  else
  { volumesound = atoi (str); }
  if (volumesound < 0) volumesound = 0;
  else if (volumesound > 100) volumesound = 100;

  str = cf->getString (ret, "music");
  if (str == NULL)
  { volumemusic = 100; }
  else
  { volumemusic = atoi (str); }
  if (volumemusic < 0) volumemusic = 0;
  else if (volumemusic > 100) volumemusic = 100;
#endif

  str = cf->getString (ret, "controls");
  if (str == NULL)
  { controls = CONTROLS_MOUSE; }
  else
  { controls = atoi (str); }
  if (controls < 0) controls = 0;
  else if (controls > 2) controls = 0;

  str = cf->getString (ret, "difficulty");
  if (str == NULL)
  { difficulty = 1; }
  else
  { difficulty = atoi (str); }
  if (difficulty < 0) difficulty = 0;
  else if (difficulty > 2) difficulty = 0;

/*  str = cf->getString (ret, "mode0");
  if (str == NULL)
  { modes [0] = -1; }
  else
  { modes [0] = atoi (str); }

  str = cf->getString (ret, "mode1");
  if (str == NULL)
  { modes [1] = -1; }
  else
  { modes [1] = atoi (str); }

  str = cf->getString (ret, "mode2");
  if (str == NULL)
  { modes [2] = -1; }
  else
  { modes [2] = atoi (str); }

  str = cf->getString (ret, "mode3");
  if (str == NULL)
  { modes [3] = -1; }
  else
  { modes [3] = atoi (str); }*/

  if (cf->buf [0] == 0) // no file found
  {
    delete cf;
    return 0;
  }

  delete cf;
  return 1;
}




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

void drawMedal (float xp, float yp, float zp, int medal, float zoom)
{
  if (medal < 0) return;
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
/*  glTexCoord2d (0, 0);
  glVertex2d (mousex - crossradius, mousey - crossradius);
  glTexCoord2d (0, 1);
  glVertex2d (mousex - crossradius, mousey + crossradius);
  glTexCoord2d (1, 1);
  glVertex2d (mousex + crossradius, mousey + crossradius);
  glTexCoord2d (1, 0);
  glVertex2d (mousex + crossradius, mousey - crossradius);*/
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




#ifndef USE_GLUT
SDL_Joystick *sdljoystick;
#endif
int sdldisplay = true;
int sdlreshape = true;
Cockpit *cockpit;
float fps = 30;

int glutwindow;

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

int setScreen (int w, int h, int b, int f)
{
  int ret = 0;
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
  int rgb_size[3];
  switch (b) {
    case 8:
    rgb_size[0] = 2;
    rgb_size[1] = 3;
    rgb_size[2] = 3;
    break;
    case 15:
    case 16:
    rgb_size[0] = 5;
    rgb_size[1] = 5;
    rgb_size[2] = 5;
    break;
    default:
    rgb_size[0] = 8;
    rgb_size[1] = 8;
    rgb_size[2] = 8;
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
  glViewport(0, 0, (GLint) w, (GLint) h);
  width = w;
  height = h;
  bpp = b;
  fullscreen = f;
//  fprintf (stderr, "\nUsing %dx%d:%d video mode. Edit the conf file to change.", width, height, bpp);
  fflush (stderr);
  return 1;
}

/*void drawTexture (float r, float p1, float t1, float p2, float t2)
{
  gl->enableTextures ();
  gl->enableAlphaBlending ();
  glDisable (GL_DITHER);
  glBegin (GL_QUADS);
  float f = atan(1.0)*4.0 / 180.0;
  float sinp1 = sin (p1*f), sinp2 = sin (p2*f), sint1 = sin (t1*f), sint2 = sin (t2*f);
  float cosp1 = cos (p1*f), cosp2 = cos (p2*f), cost1 = cos (t1*f), cost2 = cos (t2*f);
  float x = r * cosp1 * sint2, y = r * sinp1 * sint2, z = r * cost2;
  printf ("(%f,%f,%f)\n", x,y,z); fflush (stdout);
  glColor4f (1.0, 1.0, 1.0, 1.0);
  glTexCoord2d (0, 1);
  glVertex3f (x, y, z);
  x = r * cosp2 * sint2, y = r * sinp2 * sint2, z = r * cost2;
  glTexCoord2d (1, 1);
  glVertex3f (x, y, z);
  x = r * cosp2 * sint1, y = r * sinp2 * sint1, z = r * cost1;
  glTexCoord2d (1, 0);
  glVertex3f (x, y, z);
  x = r * cosp1 * sint1, y = r * sinp1 * sint1, z = r * cost1;
  glTexCoord2d (0, 0);
  glVertex3f (x, y, z);
  glEnd ();
  glDisable (GL_TEXTURE_2D);
}*/

bool initing = true;
int flash = 0;
int lastshield = 0;

void setPlaneVolume ()
{
  if (game == GAME_PLAY)
  {
    int lev = (int) ((float) 128 * fplayer->speed / fplayer->maxspeed) - 32;
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

void game_levelInit ()
{
  int i;

  initing = true;
  flash = 0;

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
    fighter [i]->recspeed = 0.2;
    fighter [i]->rectheta = 0;
    fighter [i]->o = &model_fig;
  }

  for (i = 0; i < maxgroundobj; i ++)
  {
    groundobj [i]->dinit ();
    groundobj [i]->zoom = 1;
    groundobj [i]->id = STATIC;
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
      laser [i]->zoom = 0.07;
    }
    else
    {
      laser [i]->o = &model_cannon2;
      laser [i]->zoom = 0.1;
    }
  }

  for (i = 0; i < maxmissile; i ++)
  {
    missile [i]->aiinit ();
    missile [i]->deactivate ();
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
      skycolor.setColor (127, 127, 127);
      objsphere->setColor (&skycolor);
    }
    if (sungamma < 35)
    {
      gl->fogcolor [0] = (float) (127 + 70 - 2 * sungamma) / 256.0;
    }
    else
    {
      gl->fogcolor [0] = 0.5;
    }
    gl->fogcolor [1] = 0.5;
    gl->fogcolor [2] = 0.5;
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
  initing = false;
  lastshield = fplayer->shield;

  fps = 30;

#ifdef HAVE_SDL_NET
  if (multiplayer)
  {
    if (threadnet != NULL)
      SDL_KillThread (threadnet);
    threadnet = SDL_CreateThread (net_thread_main, NULL);
  }
#endif

}

void game_reshape ()
{
  /* Darstellung auf gesamten Clientbereich des Fensters zulassen */
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  /* angle, aspect, near Clip, far Clip */
  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (80.0, (float) width / height, nearclippingplane, v); // should be sqrt(2) or 1.5
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
  /* Darstellung auf gesamten Clientbereich des Fensters zulassen */
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  /* angle, aspect, near Clip, far Clip */
  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (80.0, 1.0, nearclippingplane, v); // should be sqrt(2) or 1.5
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
  SDL_ShowCursor (0);
#endif
}

void credits_reshape ()
{
  /* Darstellung auf gesamten Clientbereich des Fensters zulassen */
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  /* angle, aspect, near Clip, far Clip */
  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (80.0, 1.0, nearclippingplane, v); // should be sqrt(2) or 1.5
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
  SDL_ShowCursor (0);
#endif
}

void stats_reshape ()
{
  /* Darstellung auf gesamten Clientbereich des Fensters zulassen */
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  /* angle, aspect, near Clip, far Clip */
  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (80.0, 1.0, nearclippingplane, v); // should be sqrt(2) or 1.5
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
  SDL_ShowCursor (0);
#endif
}

void switch_menu ()
{
  setLightSource ((int) sungamma);
  game = GAME_MENU;
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
  {
    int r = myrandom (3);
    if (r == 0)
      sound->playMusic (MUSIC_DARK1);
    else if (r == 1)
      sound->playMusic (MUSIC_STANDBY1);
    else
      sound->playMusic (MUSIC_ELECTRO1);
  }
}

bool ispromoted;
int statsitemselected = 0;

void switch_stats ()
{
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
    sound->playMusic (MUSIC_WINNER1);
}

void createMission (int missionid)
{
  if (missionid >= MISSION_MULTIPLAYER)
    multiplayer = true;
  else
  {
    multiplayer = false;
    isserver = false;
  }
  if (missionnew != NULL) delete missionnew;
  if (missionid == MISSION_DEMO) missionnew = new MissionDemo1 ();
  else if (missionid == MISSION_TUTORIAL) missionnew = new MissionTutorial1 ();
  else if (missionid == MISSION_DOGFIGHT) missionnew = new MissionDogfight1 ();
  else if (missionid == MISSION_MOON) missionnew = new MissionMoon1 ();
  else if (missionid == MISSION_CANYON) missionnew = new MissionCanyon1 ();
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
  else if (missionid == MISSION_MOON1) missionnew = new MissionMoonDefense1 ();
  else if (missionid == MISSION_MOON2) missionnew = new MissionMoonDogfight1 ();
  else if (missionid == MISSION_MOON3) missionnew = new MissionMoonBase1 ();
  else if (missionid == MISSION_MULTIPLAYER_DOGFIGHT) missionnew = new MissionMultiDogfight1 ();
}

void switch_mission (int missionid)
{
  setLightSource (60);
  game = GAME_MISSION;
  createMission (missionid);
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    sound->playMusic (MUSIC_DARK1);
}

void switch_fame ()
{
  setLightSource (60);
  game = GAME_FAME;
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    sound->playMusic (MUSIC_DARK1);
}

void switch_fighter ()
{
  setLightSource (60);
  game = GAME_FIGHTER;
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    sound->playMusic (MUSIC_DARK1);
}

void switch_create ()
{
  setLightSource (60);
  game = GAME_CREATE;
  isserver = true;
  if (server != NULL) delete server;
  server = new Server ();
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    sound->playMusic (MUSIC_DARK1);
}

void switch_join ()
{
  setLightSource (60);
  game = GAME_JOIN;
  isserver = false;
  if (client != NULL) delete client;
  client = new Client ();
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    sound->playMusic (MUSIC_DARK1);
}

int creditstimer, finishtimer;

void switch_credits ()
{
  game = GAME_CREDITS;
  creditstimer = 50;
  credits_reshape ();
/*  if (sound->musicplaying)
    sound->haltMusic ();
  sound->playMusic (MUSIC_WINNER1);*/
  sound->stop (SOUND_PLANE1);
}

void switch_finish ()
{
  game = GAME_FINISH;
  creditstimer = 50;
  credits_reshape ();
  if (sound->musicplaying)
    sound->haltMusic ();
  sound->playMusic (MUSIC_ELECTRO1);
  sound->stop (SOUND_PLANE1);
}

void switch_game ()
{
  setLightSource ((int) sungamma);
  game = GAME_PLAY;
  game_reshape ();
  if (sound->musicplaying)
    sound->haltMusic ();
  sound->playLoop (SOUND_PLANE1);
  setPlaneVolume ();
}

void game_key (unsigned char key, int x, int y)
{
  if (camera == 50 && game == GAME_PAUSE)
  {
    camera = 0;
    game = GAME_PLAY;
    return;
  }
  if (key == 'v')
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
  }
  else if (key == 27)
  {
    switch_menu ();
  }
  else if (key == 13)
  {
    fplayer->fireMissile (fplayer->missiletype + MISSILE1, missile);
    sound->play (SOUND_MISSILE1);
  }
  else if (key == ' ')
  {
#ifdef USE_GLUT
    fplayer->fireCannon (laser);
    sound->play (SOUND_CANNON1);
#else
    if (fplayer->autofire)
      fplayer->autofire = false;
    else
      fplayer->autofire = true;
#endif
  }
    else if (key == 'm')
    {
      fplayer->missiletype = fplayer->nextMissile (fplayer->missiletype);
//      printf ("%d missiles left\n", fplayer->missiles);
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
      fplayer->recspeed = fplayer->maxspeed * (1.0 / 18.0 * (key - '0') + 0.5);
      sound->play (SOUND_CLICK1);
    }
/*    else if (key == 'a')
    {
      fplayer->ai = !fplayer->ai;
      fplayer->easymodel = fplayer->ai;
      if (controls == 100)
        fplayer->easymodel = true;
    }*/
    else if (key == 'p')
    {
/*      printf ("pos: x=%f, z=%f\n", fplayer->tl->x, fplayer->tl->z);
      printf ("mouse: x=%d, y=%d\n", mousex, mousey);
      printf ("gamma: f=%d, cam=%d\n", (int) fplayer->gamma, (int) camgamma);*/
//      printf ("speed: f=%f\n", fplayer->speed);
//      fflush (stdout);
      if (game == GAME_PLAY) game = GAME_PAUSE;
      else game = GAME_PLAY;
    }
/*    else if (key == 'w')
    {
      fplayer->speedUp ();
      sound->play (SOUND_CLICK1);
      setPlaneVolume ();
    }
    else if (key == 's')
    {
      fplayer->speedDown ();
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
    else if (key == 'e')
    {
      fplayer->targetNearestEnemy ((AIObj **) fighter);
//      explosion [0]->setExplosion (fplayer->tl->x, fplayer->tl->y, fplayer->tl->z, 1.0);
//      fplayer->explode ++;
      sound->play (SOUND_CLICK1);
    }
    else if (key == 't')
    {
      fplayer->targetNext ((AIObj **) fighter);
      sound->play (SOUND_CLICK1);
    }
    else if (key == 'k')
    {
      if (fplayer->target != NULL)
        fplayer->target->shield = 0;
    }
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

int aktcam = 0;

#ifdef USE_GLUT
  #define MOUSE_BUTTON_LEFT GLUT_LEFT_BUTTON
  #define MOUSE_BUTTON_MIDDLE GLUT_MIDDLE_BUTTON
  #define MOUSE_BUTTON_RIGHT GLUT_RIGHT_BUTTON
  #define MOUSE_UP GLUT_UP
  #define MOUSE_DOWN GLUT_DOWN
  #define KEY_F1 GLUT_KEY_F1
  #define KEY_F2 GLUT_KEY_F2
  #define KEY_F3 GLUT_KEY_F3
  #define KEY_F4 GLUT_KEY_F4
  #define KEY_F5 GLUT_KEY_F5
  #define KEY_F6 GLUT_KEY_F6
  #define KEY_F7 GLUT_KEY_F7
  #define KEY_F8 GLUT_KEY_F8
  #define KEY_F9 GLUT_KEY_F9
  #define KEY_UP GLUT_KEY_UP
  #define KEY_DOWN GLUT_KEY_DOWN
  #define KEY_LEFT GLUT_KEY_LEFT
  #define KEY_RIGHT GLUT_KEY_RIGHT
  #define KEY_PGUP GLUT_KEY_PAGE_UP
  #define KEY_PGDOWN GLUT_KEY_PAGE_DOWN
#else
  #define MOUSE_BUTTON_LEFT SDL_BUTTON_LEFT
  #define MOUSE_BUTTON_MIDDLE SDL_BUTTON_MIDDLE
  #define MOUSE_BUTTON_RIGHT SDL_BUTTON_RIGHT
  #define MOUSE_UP SDL_RELEASED
  #define MOUSE_DOWN SDL_PRESSED
  #define KEY_F1 SDLK_F1
  #define KEY_F2 SDLK_F2
  #define KEY_F3 SDLK_F3
  #define KEY_F4 SDLK_F4
  #define KEY_F5 SDLK_F5
  #define KEY_F6 SDLK_F6
  #define KEY_F7 SDLK_F7
  #define KEY_F8 SDLK_F8
  #define KEY_F9 SDLK_F9
  #define KEY_UP SDLK_UP
  #define KEY_DOWN SDLK_DOWN
  #define KEY_LEFT SDLK_LEFT
  #define KEY_RIGHT SDLK_RIGHT
  #define KEY_PGUP SDLK_PAGEUP
  #define KEY_PGDOWN SDLK_PAGEDOWN
#endif

void game_keyspecialup (int key, int x, int y)
{
  switch (key) {
  case ' ':
    fplayer->autofire = false;
    break;
  case KEY_UP:
//    fplayer->speedUp ();
//    sound->play (SOUND_CLICK1);
    fplayer->aileroneffect = 0.0;
    break;
  case KEY_DOWN:
//    fplayer->speedDown ();
//    sound->play (SOUND_CLICK1);
    fplayer->aileroneffect = 0.0;
    break;
  case KEY_LEFT:
    fplayer->rolleffect = 0;
//    fplayer->rectheta = fplayer->theta;
    break;
  case KEY_RIGHT:
    fplayer->rolleffect = 0;
//    fplayer->rectheta = fplayer->theta;
    break;
#ifdef HAVE_SDL
  case KEY_PGUP:
    fplayer->ruddereffect = 0;
    break;
  case KEY_PGDOWN:
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
//    fplayer->speedUp ();
//    sound->play (SOUND_CLICK1);
    fplayer->aileroneffect = -0.5;
    break;
  case KEY_DOWN:
//    fplayer->speedDown ();
//    sound->play (SOUND_CLICK1);
    fplayer->aileroneffect = 1.0;
    break;
  case KEY_LEFT:
    fplayer->rolleffect = 10;
    break;
  case KEY_RIGHT:
    fplayer->rolleffect = -10;
    break;
  case KEY_PGUP:
    fplayer->ruddereffect = 1.0;
    break;
  case KEY_PGDOWN:
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
//    aktcam ++;
//    if (aktcam > 7) aktcam = 0;
    game_reshape ();
    break;
  case KEY_F5:
    camera = 7;
    game_reshape ();
    break;
  case KEY_F6:
    camera = 4;
    game_reshape ();
    break;
  case KEY_F7:
    camera = 5;
    game_reshape ();
    break;
  case KEY_F8:
    camera = 50;
    game_reshape ();
    break;
  case KEY_F9:
    aktcam ++;
    if (aktcam > 7) aktcam = 0;
    camera = 3;
    break;
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
  if (state == MOUSE_DOWN)
  {
    if (button == MOUSE_BUTTON_LEFT)
    {
//      fplayer->fireCannon (laser);
//      sound->play (SOUND_CANNON1);
      fplayer->autofire = !fplayer->autofire;
    }
    else if (button == MOUSE_BUTTON_RIGHT)
    {
      if (fplayer->missiles [fplayer->missiletype] > 0)
      {
        fplayer->fireMissile (fplayer->missiletype + MISSILE1, missile);
        sound->play (SOUND_MISSILE1);
      }
    }
  }
}

float dtheta = 0, dgamma = 0;

/* Hi Bernd. To test the messy mouse interface code you will have to do two more things,
   as the code has been disabled on the CVS server.
   Just search on for your first (short) name! */
int lastmousex = 0;

// This function is experimental. It is called about 30 times per second.
void game_easymouse ()
{
  float mx = width / 2, my = height / 2;
  float dx = mousex - mx, dy = my - mousey; // deltax and deltay

  int t = (int) fplayer->theta; // roll angle
  if (t < 0) t += 360;
  float rx = dx * cosi [t] - dy * sine [t]; // rolled mouse x coordinate
  float ry = -dx * sine [t] + dy * cosi [t]; // rolled mouse y coordinate

  if (mousex != lastmousex) // only change roll angle on mouse event
  {
    fplayer->rectheta = -rx * 240 / width; // some scaling
  }
  if (fplayer->rectheta > 90) fplayer->rectheta = 90; // limits
  else if (fplayer->rectheta < -90) fplayer->rectheta = -90;
  lastmousex = mousex; // save last mouse x coordinate to detect an event

  // calculate the aileron effect every time
  fplayer->aileroneffect = 0.005F * ry;
  if (fplayer->aileroneffect > 1.0) fplayer->aileroneffect = 1.0;
  else if (fplayer->aileroneffect < -0.5) fplayer->aileroneffect = -0.5;
}

/*void sdl_mousemotion (int xrel, int yrel)
{
  fplayer->rectheta -= xrel / 2;
  fplayer->aileroneffect += 0.005F * yrel;
  if (fplayer->aileroneffect > 1.0) fplayer->aileroneffect = 1.0;
  if (fplayer->aileroneffect < -0.5) fplayer->aileroneffect = -0.5;
}*/

void game_mousemotion (int x, int y)
{
  if (controls != CONTROLS_MOUSE) return;

/* Bernd: Please activate the following return to disable this method! */
//  return;

  float f = (float) width / 240.0;
  float mx = width / 2, my = height / 2;
  float dx = x - mx, dy = my - y;

/*  int t = (int) fplayer->theta;
  if (t < 0) t += 360;
  float rx = dx * cosi [t] - dy * sine [t];
  float ry = dx * sine [t] + dy * cosi [t];
  rx += mx;
  ry = my - ry;*/
//  fplayer->rectheta = (float) (width / 2 - rx) / f;
//  fplayer->recgamma = 135.0 + (float) (height - ry) / height * 90.0;

  // innerhalb width/15 und height/15 vom Mittelpunkt aus reagiert nur das Ruder
  // fragt sich, inwieweit height/15 sinnvoll ist... Ruder ist hier für die Feineinstellung
  if (fabs (dx) <= width / 15 && fabs (dy) <= height / 15)
  {
    fplayer->ruddereffect = (float) dx / width * 15.0; // damit ist automatisch -1<=ruddereffect<=1
    fplayer->rolleffect = 0;
  }
  else if (fabs (dx) <= width / 15)
  {
    fplayer->rolleffect = 0;
  }
  else
  {
    if (dx > 0) dx -= width / 15;
    else dx += width / 15;
    fplayer->rolleffect = (float) -dx / width * 15.0;
  }
  fplayer->aileroneffect = dy / height * 2.5;
  if (fplayer->aileroneffect > 1.0)
    fplayer->aileroneffect = 1.0;
  else if (fplayer->aileroneffect < -0.5)
    fplayer->aileroneffect = -0.5;

#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void game_joystickaxis (int x, int y, int throttle, int rudder)
{
  if (fplayer->ai) return;
/*  int t = (int) fplayer->theta;
  if (t < 0) t += 360;
  float rx = x * cosi [t] - y * sine [t];
  float ry = x * sine [t] + y * cosi [t];*/
  fplayer->rectheta -= (float) x / 7500;
/*  fplayer->recgamma += (float) ry / 5000;
  if (fplayer->recgamma > 225) fplayer->recgamma = 225;
  if (fplayer->recgamma < 135) fplayer->recgamma = 135;*/
  fplayer->aileroneffect = (float) y / 30000;
  if (fplayer->aileroneffect > 1.0) fplayer->aileroneffect = 1.0;
  else if (fplayer->aileroneffect < -1.0) fplayer->aileroneffect = -1.0;
  if (fplayer->aileroneffect < 0) fplayer->aileroneffect /= 2;
  fplayer->ruddereffect = (float) rudder / 30000;
  if (fplayer->ruddereffect > 1.0) fplayer->ruddereffect = 1.0;
  else if (fplayer->ruddereffect < -1.0) fplayer->ruddereffect = -1.0;
//  fplayer->aileroneffect = 0;
  fplayer->recspeed = fplayer->maxspeed * 0.75 - fplayer->maxspeed / 4 * throttle / 32638;
}

void game_joystickbutton (int button)
{
  if (button == 0)
  {
    fplayer->fireCannon (laser);
    sound->play (SOUND_CANNON1);
  }
  else if (button == 1)
  {
    fplayer->targetNearestEnemy ((AIObj **) fighter);
    sound->play (SOUND_CLICK1);
  }
  else if (button == 2)
  {
    if (fplayer->missiles [fplayer->missiletype] > 0)
    {
      fplayer->fireMissile (fplayer->missiletype + MISSILE1, missile);
      sound->play (SOUND_MISSILE1);
    }
  }
  else if (button == 3)
  {
    fplayer->missiletype = fplayer->nextMissile (fplayer->missiletype);
    sound->play (SOUND_CLICK1);
  }
}

void game_joystickhat (int hat)
{
#ifndef USE_GLUT
  if (hat == SDL_HAT_LEFT)
  {
    fplayer->targetPrevious ((AIObj **) fighter);
    sound->play (SOUND_CLICK1);
  }
  else if (hat == SDL_HAT_RIGHT)
  {
    fplayer->targetNext ((AIObj **) fighter);
    sound->play (SOUND_CLICK1);
  }
#endif
}

static void myDisplayFunc2 ()
{
//  RenderScene ();
//  glutSwapBuffers();
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

  CColor color (255, 255, 255, 255);
  font1->drawTextCentered (0, -0.5, -1.5, "PLEASE WAIT...", &color);
  game_view ();
}

void stats_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    pleaseWait ();
    printf ("initing new mission"); fflush (stdout);
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

void fighter_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    switch_menu ();
  }
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
  else if (id == MISSILE_GROUND1) return &model_missile4;
  else if (id == MISSILE_DF1) return &model_missile6;
  return &model_fig;
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
  int lastitem = menuitem;
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

void mission_display ()
{
  char buf [256];
  int i;
  CColor colorblue (100, 150, 255, 255);
  CColor colorgreen (100, 255, 100, 255);
  CColor colororange (255, 150, 100, 255);
  CColor colorred (255, 0, 0, 255);
  float xstats = 2, ystats = 6;
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
  float zf = -3, yf;
  for (i = 0; i < 14; i ++)
  {
    yf = -3 + 0.5 * i - (float) (missionmenutimer & 63) / 64.0;
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 14; i2 ++)
    {
      float cola = sine [(i * 100+missionmenutimer*4) % 360] / 10 + sine [(i2 * 100) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (-3 + 0.5 * i2, -3 + 0.5 * i, zf + sine [(i * 100) % 360] / 2);
      cola = sine [((i+1) * 100+missionmenutimer*4) % 360] / 10 + sine [(i2 * 100) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (-3 + 0.5 * i2, -2.5 + 0.5 * i, zf + sine [((i+1) * 100) % 360] / 2);
    }
    glEnd ();
/*    glBegin (GL_QUADS);
    glColor3ubv (colorstd->c);
    glVertex3f (-3, yf, zf);
    glColor3ub (0, 0, 0);
    glVertex3f (-3, yf + 0.25, zf);
    glVertex3f (-2, yf + 0.25, zf);
    glVertex3f (-2, yf, zf);
    glColor3ubv (colorstd->c);
    glVertex3f (-3, yf, zf);
    glColor3ub (0, 0, 0);
    glVertex3f (-3, yf - 0.25, zf);
    glVertex3f (-2, yf - 0.25, zf);
    glVertex3f (-2, yf, zf);
    glColor3ubv (colorstd->c);
    glVertex3f (3, yf, zf);
    glColor3ub (0, 0, 0);
    glVertex3f (3, yf + 0.25, zf);
    glVertex3f (2, yf + 0.25, zf);
    glVertex3f (2, yf, zf);
    glColor3ubv (colorstd->c);
    glVertex3f (3, yf, zf);
    glColor3ub (0, 0, 0);
    glVertex3f (3, yf - 0.25, zf);
    glVertex3f (2, yf - 0.25, zf);
    glVertex3f (2, yf, zf);
    glEnd ();*/
  }

/*  xf = missionnew->wantfighter - 2.6;
  yf = -1.5;
  zf = -3.5;
  glBegin (GL_QUADS);
  glColor3ub (0, 100, 0);
  glVertex3f (xf, yf, zf);
  glVertex3f (xf + 0.9, yf, zf);
  glColor3ub (0, 0, 0);
  glVertex3f (xf + 0.9, yf + 0.5, zf);
  glVertex3f (xf, yf + 0.5, zf);
  glEnd ();*/

  CVector3 vec;
  CVector3 tl (-4.5, 5.5, -8.0);
  CRotation rot;
  rot.a = 270;
  rot.b = (270 - missionmenutimer * 4) % 360;
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
    if (missionnew->wantfighter == i/*missionmenufighterselected == i*/)
      rot.c = (5 + missionmenutimer * 4) % 360;
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
    if (missionnew->wantweapon == i/*missionmenufighterselected == i*/)
      rot.c = (5 + missionmenutimer * 4) % 360;
    else
      rot.c = 5;
    getModel (missionnew->selweapon [i])->draw2 (&vec, &tl, &rot, 0.9, 0);
  }
  glDisable (GL_DEPTH_TEST);
  glDisable (GL_LIGHTING);

/*  for (i = 0; i < 8; i ++)
  {
    xf = -2.5 + 0.5 * (i & 3);
    yf = -0.5 - 0.92 * (i / 4);
    zf = -3.5;
    gl->enableTextures (texchar [i]->textureID);
    glBegin (GL_QUADS);
    glColor3ub (255, 255, 255);
    glTexCoord2d (0, 0);
    glVertex3f (xf, yf, zf);
    glTexCoord2d (1, 0);
    glVertex3f (xf + 0.45, yf, zf);
    glTexCoord2d (1, 1);
    glVertex3f (xf + 0.45, yf + 0.85, zf);
    glTexCoord2d (0, 1);
    glVertex3f (xf, yf + 0.85, zf);
    glEnd ();
    glDisable (GL_TEXTURE_2D);
  }*/

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
  font1->drawText (xstats, ystats - 1.5, -3, "SCORE:");
  int score = p->mission_score [missionnew->id];
  if (score < -10000 || score > 100000) score = 0;
  sprintf (buf, "%d", score);
  font1->drawText (xstatstab, ystats - 1.5, -3, buf, colorstd);
  font1->drawText (xstats, ystats - 3, -3, "KILLS:");
  sprintf (buf, "%d AIRCRAFTS", p->mission_fighterkills [missionnew->id]);
  font1->drawText (xstatstab, ystats - 3, -3, buf, colorstd);
  
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
    font1->drawTextRotated (2, -14, -2, "START MISSION", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (2, -14, -2, "START MISSION");
  if (missionmenuitemselected == 1)
    font1->drawTextRotated (-12, -14, -2, "CANCEL", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-12, -14, -2, "CANCEL");
  font2->drawText (-20, -14, -3, getModelName (missionnew->selfighter [missionnew->wantfighter]));
  font2->drawText (xstats + 2, -14, -3, getModelName (missionnew->selweapon [missionnew->wantweapon]));
/*  if (missionmenufighterselected > -1)
  {
    float textx = xstats;
    if (missionnew->alliedfighters < 3)
      textx -= 8;
    font2->drawText (textx, -7, -3, getModelText (missionnew->selfighter [missionmenufighterselected]));
  }*/
  glPopMatrix ();

  drawMouseCursor ();
}

void fame_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  int lastitemselected = missionmenuitemselected;
  int lastitem = menuitem;
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

int aktfighter = 0;

void fighter_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  int lastitemselected = missionmenuitemselected;
  int lastitem = menuitem;
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
  int i;
  CColor colorblue (100, 150, 255, 255);
  Pilot *p = pilots->pilot [pilots->aktpilot];
  CColor *colorstd = &colorblue;

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();
  float zf = -3, yf;
  for (i = 0; i < 14; i ++)
  {
    yf = -3 + 0.5 * i - (float) (missionmenutimer & 63) / 64.0;
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 14; i2 ++)
    {
      float cola = sine [(i * 100+missionmenutimer*4) % 360] / 10 + sine [(i2 * 100) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (-3 + 0.5 * i2, -3 + 0.5 * i, zf + sine [(i * 100) % 360] / 2);
      cola = sine [((i+1) * 100+missionmenutimer*4) % 360] / 10 + sine [(i2 * 100) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (-3 + 0.5 * i2, -2.5 + 0.5 * i, zf + sine [((i+1) * 100) % 360] / 2);
    }
    glEnd ();
  }

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
  rot.a = 240;
  rot.b = 0;
  rot.c = (5 + missionmenutimer * 4) % 360;
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
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  model->draw2 (&vec, &tl, &rot, 1.0, 0);
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);

  fplayer->newinit (id, 1, 0);

  font1->drawText (-10, 9, -2, getModelName (id));
  yf = 9.5;
  strcpy (buf, "TYPE: ");
  if (fplayer->id == FIGHTER_FALCON || fplayer->id == FIGHTER_CROW || fplayer->id == FIGHTER_BUZZARD || fplayer->id == FIGHTER_REDARROW || fplayer->id == FIGHTER_BLACKBIRD)
    strcat (buf, "FIGHTER");
  else
    strcat (buf, "BOMBER");
  font1->drawText (-10, yf, -2.5, buf);
  yf -= 1.5;
  strcpy (buf, "SPEED: ");
  if (fplayer->maxspeed < 0.20) strcat (buf, "VERY SLOW");
  else if (fplayer->maxspeed < 0.25) strcat (buf, "SLOW");
  else if (fplayer->maxspeed < 0.3) strcat (buf, "INTERMEDITATE");
  else if (fplayer->maxspeed < 0.33) strcat (buf, "FAST");
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
    font1->drawTextRotated (-2, -12, -2, "BACK", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-2, -12, -2, "BACK");

  glPopMatrix ();

  drawMouseCursor ();
}

void fame_display ()
{
  char buf [256];
  int i, i2;
  CColor colorblue (100, 150, 255, 255);
  Pilot *p = pilots->pilot [pilots->aktpilot];
  CColor *colorstd = &colorblue;

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();
  float zf = -3, yf, xf;
  int t = 360 - (missionmenutimer * 10) % 360;
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

  font1->drawTextCentered (0, 8, -1.5, "TOP PILOTS");
  int sum = 0;
  for (i = 0; i < 100; i ++)
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
    font1->drawTextRotated (-2, -10, -2, "BACK", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-2, -10, -2, "BACK");

  glPopMatrix ();

  drawMouseCursor ();
}

CModel *explsphere;
CSphere *mysphere;
InitKugel *mykugel [100];

void game_quit ()
{
  int i;
  volumesound = sound->volumesound;
  volumemusic = sound->volumemusic;
  save_config ();
  pilots->save (dirs->getSaves ("pilots"));
  for (i = 0; i < maxfighter; i ++)
    delete (fighter [i]);
  for (i = 0; i < maxlaser; i ++)
    delete (laser [i]);
  for (i = 0; i < maxmissile; i ++)
    delete (missile [i]);
  for (i = 0; i < maxexplosion; i ++)
    delete (explosion [i]);
  for (i = 0; i < maxstar; i ++)
    delete (star [i]);
  for (i = 0; i < maxgroundobj; i ++)
    delete (groundobj [i]);
  for (i = 0; i < maxblacksmoke; i ++)
    delete (blacksmoke [i]);
  for (i = 0; i < 100; i++)
    delete (mykugel [i]);
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
// free allocated memory (optional)
#ifndef USE_GLUT
/*  SDL_CloseAudio();
  SDL_FreeWAV(wave.sound);*/
  SDL_Quit ();
  delete sound;
#endif
  printf ("\n"); fflush (stdout);
  exit (0);
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
#ifdef HAVE_SDL_NET
    else if (ry >= 0.28 && ry <= 0.33)
    {
      menuitemselected = 2;
      if (state == MOUSE_DOWN)
        menuitem = 2;
    }
#endif
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
        game_quit ();
      }
    }
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
    Pilot *p = pilots->pilot [pilots->aktpilot];
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
      if (ry >= 0.18 && ry <= 0.20)
      {
        menuitemselected = 12;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_TRANSPORT);
        }
      }
      if (ry >= 0.21 && ry <= 0.23 && p->mission_state [MISSION_TRANSPORT] == 1)
      {
        menuitemselected = 13;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_CONVOY);
        }
      }
      if (ry >= 0.24 && ry <= 0.26 && p->mission_state [MISSION_CONVOY] == 1)
      {
        menuitemselected = 14;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_DOGFIGHT2);
        }
      }
      if (ry >= 0.27 && ry <= 0.29 && p->mission_state [MISSION_DOGFIGHT2] == 1)
      {
        menuitemselected = 15;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_AIRBATTLE);
        }
      }
      if (ry >= 0.30 && ry <= 0.32 && p->mission_state [MISSION_AIRBATTLE] == 1)
      {
        menuitemselected = 16;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_SADEFENSE);
        }
      }
      if (ry >= 0.33 && ry <= 0.35 && p->mission_state [MISSION_SADEFENSE] == 1)
      {
        menuitemselected = 17;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_SCOUT);
        }
      }
      if (ry >= 0.36 && ry <= 0.38 && p->mission_state [MISSION_SCOUT] == 1)
      {
        menuitemselected = 18;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_BASE);
        }
      }
      if (ry >= 0.39 && ry <= 0.41 && p->mission_state [MISSION_BASE] == 1)
      {
        menuitemselected = 20;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_DEFEND1);
        }
      }
      if (ry >= 0.42 && ry <= 0.44 && p->mission_state [MISSION_DEFEND1] == 1)
      {
        menuitemselected = 21;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_DOGFIGHT3);
        }
      }
      if (ry >= 0.45 && ry <= 0.47 && p->mission_state [MISSION_DOGFIGHT3] == 1)
      {
        menuitemselected = 22;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_TANK1);
        }
      }
      if (ry >= 0.48 && ry <= 0.50 && p->mission_state [MISSION_TANK1] == 1)
      {
        menuitemselected = 25;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_SHIP1);
        }
      }
      if (ry >= 0.51 && ry <= 0.53 && p->mission_state [MISSION_SHIP1] == 1)
      {
        menuitemselected = 26;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_SHIP2);
        }
      }
      if (ry >= 0.54 && ry <= 0.56 && p->mission_state [MISSION_SHIP2] == 1)
      {
        menuitemselected = 27;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_SHIP3);
        }
      }
      if (ry >= 0.57 && ry <= 0.59 && p->mission_state [MISSION_SHIP3] == 1)
      {
        menuitemselected = 30;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_CANYON1);
        }
      }
      if (ry >= 0.60 && ry <= 0.62 && p->mission_state [MISSION_CANYON1] == 1)
      {
        menuitemselected = 31;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_CANYON2);
        }
      }
      if (ry >= 0.63 && ry <= 0.65 && p->mission_state [MISSION_CANYON2] == 1)
      {
        menuitemselected = 32;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_CANYON3);
        }
      }
      if (ry >= 0.66 && ry <= 0.68 && p->mission_state [MISSION_CANYON3] == 1)
      {
        menuitemselected = 33;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_MOON1);
        }
      }
      if (ry >= 0.69 && ry <= 0.71 && p->mission_state [MISSION_MOON1] == 1)
      {
        menuitemselected = 34;
        if (state == MOUSE_DOWN)
        {
          switch_mission (MISSION_MOON2);
        }
      }
      if (ry >= 0.72 && ry <= 0.74 && p->mission_state [MISSION_MOON2] == 1)
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

#ifdef HAVE_SDL_NET
  if (menuitem == 2)
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
            if (view > 100) view = 20;
            menu_reshape ();
          }
          else
          {
            view -= 10;
            if (view < 20) view = 100;
            menu_reshape ();
          }
        }
      }
      else if (ry >= 0.28 && ry <= 0.33)
      {
        menuitemselected = 12;
        if (state == MOUSE_DOWN)
        {
          if (button == MOUSE_BUTTON_LEFT)
          {
            fplayer->rolleffect = 0;
            controls ++;
            if (controls > 2) controls = 0;
            if (controls == 2 && !joystick) controls = 0;
#ifdef USE_GLUT
            if (controls == 0) controls = 1;
#endif
          }
        }
      }
#ifdef HAVE_SDL_MIXER
      else if (ry >= 0.34 && ry <= 0.39)
      {
        menuitemselected = 13;
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
      else if (ry >= 0.40 && ry <= 0.45)
      {
        menuitemselected = 14;
        if (state == MOUSE_DOWN)
        {
          if (button == MOUSE_BUTTON_LEFT)
          {
            sound->volumemusic += 10;
            if (sound->volumemusic > 100)
              sound->volumemusic = 0;
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
        }
      }
#endif
      else if (ry >= 0.46 && ry <= 0.51)
      {
        menuitemselected = 15;
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
/*      else if (ry >= 0.52 && ry <= 0.57)
      {
        menuitemselected = 16;
        if (state == MOUSE_DOWN)
        {
          if (button == MOUSE_BUTTON_LEFT)
          {
            int i;
            for (i = 0; i < 4; i ++)
              if (resolution [i] [0] == width)
                break;
            if (i == 4) i = 0;
            do
            {
              i ++;
            } while (modes [i] == -1 && i < 4);
            if (i == 4) i = 0;
            int lastw = width;
            int lasth = height;
            int lastb = bpp;
            int lastf = fullscreen;
            width = resolution [i] [0];
            height = resolution [i] [1];
            bpp = resolution [i] [2];
            fullscreen = resolution [i] [3];
            if (!setScreen (width, height, bpp, fullscreen))
            {
              width = lastw;
              height = lasth;
              bpp = lastb;
              fullscreen = lastf;
              if (!setScreen (width, height, bpp, fullscreen))
              {
                fprintf (stderr, "\nFatal: Could neither switch to desired mode, nor back!");
                fflush (stderr);
                exit (2);
              }
            }
#ifdef USE_GLUT
            glutReshape ();
#else
            sdlreshape = true;
#endif
          }
        }
      }*/
    }
  }

  if (lastitemselected != menuitemselected)
  {
    menutimer = 0;
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

void drawMissionElement (float x, float y, float z, int thismissionid, int missionid, int selected, char *string)
{
  CColor color2 ((int) (255.0 * cosi [menutimer * 5 % 360]), 255, (int) (255.0 * cosi [menutimer * 10 % 360]), 255);
  CColor coloryellow (255, 255, 0, 200);
  CColor colorgrey (150, 150, 150, 200);
  Pilot *p = pilots->pilot [pilots->aktpilot];
  if (p->mission_state [missionid] == 1)
  {
    if (menuitemselected == selected)
      font1->drawTextRotated (x, y, z, string, &color2, -menutimer * 5);
    else
      font1->drawText (x, y, z, string);
  }
  else
  {
    font1->drawText (x, y, z, string, &colorgrey);
  }
  drawMedal (x - 0.8, y + 0.6, z, getMedal (p->mission_score [thismissionid]), 1.0);
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

  drawMedal (-5.5, 6.5, -1.5, getMedal (fplayer->score), 1.6);
  drawMedal (5.5, 6.5, -1.5, getMedal (fplayer->score), 1.6);

  char buf [100];
  float xf1 = -12, xf2 = 0;
  yf = 6; zf = -2.5;
  CColor colorwhite (255, 255, 255, 255);
  CColor colorred (255, 180, 150, 255);
  CColor colorblue (150, 180, 255, 255);
  CColor coloryellow (255, 255, 0, 255);
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
  int shieldbonus = fplayer->shield * 100 / fplayer->maxshield;
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
    font1->drawTextRotated (-3.5, yf, -2.0, "CONTINUE", &coloryellow, -menutimer * 5);
  drawMouseCursor ();
}

void menu_display ()
{
  char buf [100];
  int i;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

/*  if (quality >= 2)
    gl->enableAntiAliasing ();*/

  CColor color2 ((int) (255.0 * cosi [menutimer * 5 % 360]), 255, (int) (255.0 * cosi [menutimer * 10 % 360]), 255);
  CColor coloryellow (255, 255, 0, 200);

  int textx = -14, textx2 = 0;

  if (menuitemselected == 0)
    font1->drawTextRotated (textx, 10, -2, "PILOTS", &color2, -menutimer * 5);
  else
    font1->drawText (textx, 10, -2, "PILOTS");
  if (menuitemselected == 1)
    font1->drawTextRotated (textx, 8, -2, "MISSIONS", &color2, -menutimer * 5);
  else
    font1->drawText (textx, 8, -2, "MISSIONS");
#ifdef HAVE_SDL_NET
  if (menuitemselected == 2)
    font1->drawTextRotated (textx, 6, -2, "MULTIPLAYER", &color2, -menutimer * 5);
  else
    font1->drawText (textx, 6, -2, "MULTIPLAYER");
#endif
  if (menuitemselected == 3)
    font1->drawTextRotated (textx, 4, -2, "OPTIONS", &color2, -menutimer * 5);
  else
    font1->drawText (textx, 4, -2, "OPTIONS");
  if (menuitemselected == 4)
    font1->drawTextRotated (textx, 2, -2, "HELP", &color2, -menutimer * 5);
  else
    font1->drawText (textx, 2, -2, "HELP");
  if (menuitemselected == 5)
    font1->drawTextRotated (textx, 0, -2, "CREDITS", &color2, -menutimer * 5);
  else
    font1->drawText (textx, 0, -2, "CREDITS");
  if (menuitemselected == 6)
    font1->drawTextRotated (textx, -2, -2, "QUIT", &color2, -menutimer * 5);
  else
    font1->drawText (textx, -2, -2, "QUIT");
  if (missionactive)
  {
    if (menuitemselected == 9)
      font1->drawTextRotated (textx, -6, -2, "RETURN", &color2, -menutimer * 5);
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
        font1->drawTextRotated (textx2 + 3, 12.5 - (float) i * 3, -3, pilots->pilot [i]->getRank (), &color2, -menutimer * 5);
        font1->drawTextRotated (textx2 + 5, 11.5 - (float) i * 3, -3, pilots->pilot [i]->name, &color2, -menutimer * 5);
      }
      else
      {
        font1->drawText (textx2 + 3, 12.5 - (float) i * 3, -3, pilots->pilot [i]->getRank ());
        font1->drawText (textx2 + 5, 11.5 - (float) i * 3, -3, pilots->pilot [i]->name);
      }
    }
    if (menuitemselected == 10)
      font1->drawTextRotated (textx2 - 2, -5, -2.5, "DELETE (RIGHT MB)", &color2, -menutimer * 5);
    else
      font1->drawText (textx2 - 2, -5, -2.5, "DELETE (RIGHT MB)");
    if (menuitemselected == 11)
      font1->drawTextRotated (textx2 - 2, -7, -2.5, "CREATE", &color2, -menutimer * 5);
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
      font1->drawTextRotated (textx2, yf, zf, "TUTORIAL: PILOTING", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "TUTORIAL: PILOTING");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_TUTORIAL]), 1.0);
    yf -= 1.5;
    if (menuitemselected == 11)
      font1->drawTextRotated (textx2, yf, zf, "TUTORIAL: DOGFIGHT", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "TUTORIAL: DOGFIGHT");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_DOGFIGHT]), 1.0);
    yf -= 1.5;
    if (menuitemselected == 12)
      font1->drawTextRotated (textx2, yf, zf, "TRANSPORT", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, yf, zf, "TRANSPORT");
    drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [MISSION_TRANSPORT]), 1.0);
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
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_CANYON3, MISSION_CANYON2, 32, "MAIN BASE");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_MOON1, MISSION_CANYON3, 33, "TURRETS");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_MOON2, MISSION_MOON1, 34, "ELITE DOGFIGHT");
    drawMissionElement (textx2, yf -= 1.5, zf, MISSION_MOON3, MISSION_MOON2, 35, "SNEAKING");
    zf = -2;
    if (menuitemselected == 100)
      font1->drawTextRotated (-2, -12, zf, "PILOTS", &color2, -menutimer * 5);
    else
      font1->drawText (-2, -12, zf, "PILOTS");
    zf = -2;
    if (menuitemselected == 101)
      font1->drawTextRotated (6, -12, zf, "FIGHTER", &color2, -menutimer * 5);
    else
      font1->drawText (6, -12, zf, "FIGHTER");
/*    if (p->mission_state [MISSION_CONVOY] == 1)
    {
      if (menuitemselected == 14)
        font1->drawTextRotated (textx2, yf, zf, "DOGFIGHT", &color2, -menutimer * 5);
      else
        font1->drawText (textx2, yf, zf, "DOGFIGHT");
    }
    yf -= 1.5;
    if (p->mission_state [MISSION_DOGFIGHT2] == 1)
    {
      if (menuitemselected == 15)
        font1->drawTextRotated (textx2, yf, zf, "AIR BATTLE", &color2, -menutimer * 5);
      else
        font1->drawText (textx2, yf, zf, "AIR BATTLE");
    }
    yf -= 1.5;
    if (p->mission_state [MISSION_AIRBATTLE] == 1)
    {
      if (menuitemselected == 16)
        font1->drawTextRotated (textx2, yf, zf, "SURFACE-AIR DEFENSE", &color2, -menutimer * 5);
      else
        font1->drawText (textx2, yf, zf, "SURFACE-AIR DEFENSE");
    }
    yf -= 1.5;
    if (p->mission_state [MISSION_SADEFENSE] == 1)
    {
      if (menuitemselected == 17)
        font1->drawTextRotated (textx2, yf, zf, "DOGFIGHT VETERAN", &color2, -menutimer * 5);
      else
        font1->drawText (textx2, yf, zf, "DOGFIGHT VETERAN");
    }
    yf -= 1.5;
    if (p->mission_state [MISSION_SCOUT] == 1)
    {
      if (menuitemselected == 18)
        font1->drawTextRotated (textx2, yf, zf, "BASE ATTACK", &color2, -menutimer * 5);
      else
        font1->drawText (textx2, yf, zf, "BASE ATTACK");
    }*/
//    font1->drawText (-20, -15, -3, "MORE MISSIONS WITH THE NEXT RELEASE");
  }
  else if (menuitem == 3)
  {
    sprintf (buf, "QUALITY: %d", quality);
    if (menuitemselected == 10)
      font1->drawTextRotated (textx2, 10, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, 10, -2, buf);
    sprintf (buf, "VIEW: %d", (int) view);
    if (menuitemselected == 11)
      font1->drawTextRotated (textx2, 8, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, 8, -2, buf);
    strcpy (buf, "CONTROLS: ");
    if (controls == CONTROLS_KEYBOARD) strcat (buf, "KEYBOARD");
    else if (controls == CONTROLS_MOUSE) strcat (buf, "MOUSE");
    else if (controls == CONTROLS_JOYSTICK) strcat (buf, "JOYSTICK");
    if (menuitemselected == 12)
      font1->drawTextRotated (textx2, 6, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, 6, -2, buf);
#ifdef HAVE_SDL_MIXER
    sprintf (buf, "SOUND: %d%%", (int) sound->volumesound);
    if (menuitemselected == 13)
      font1->drawTextRotated (textx2, 4, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, 4, -2, buf);
    sprintf (buf, "MUSIC: %d%%", (int) sound->volumemusic);
    if (menuitemselected == 14)
      font1->drawTextRotated (textx2, 2, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, 2, -2, buf);
#endif
    strcpy (buf, "DIFFICULTY: ");
    if (difficulty == 0) strcat (buf, "EASY");
    else if (difficulty == 1) strcat (buf, "MEDIUM");
    else if (difficulty == 2) strcat (buf, "HARD");
    if (menuitemselected == 15)
      font1->drawTextRotated (textx2, 0, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, 0, -2, buf);
/*    sprintf (buf, "MODE: %d\x%d", width, height);
    if (menuitemselected == 16)
      font1->drawTextRotated (textx2, -2, -2, buf, &color2, -menutimer * 5);
    else
      font1->drawText (textx2, -2, -2, buf);*/
  }
  else if (menuitem == 2)
  {
#ifdef HAVE_SDL_NET
    if (menuitemselected == 10)
      font1->drawTextRotated (textx2, 10, -2, "CREATE GAME", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, 10, -2, "CREATE GAME");
    if (menuitemselected == 11)
      font1->drawTextRotated (textx2, 8, -2, "JOIN GAME", &color2, -menutimer * 5);
    else
      font1->drawText (textx2, 8, -2, "JOIN GAME");
#endif
  }
  else if (menuitem == 4)
  {
    int xs = -4, ys = 15;
    font1->drawText (xs, ys --, -3, "KEYS:");
    font1->drawText (xs, ys --, -3, "T\tNEXT TARGET");
    font1->drawText (xs, ys --, -3, "E\tTARGET NEAREST ENEMY");
    font1->drawText (xs, ys --, -3, "M\tSWITCH WEAPON");
    font1->drawText (xs, ys --, -3, "1..9\tCHANGE SPEED");
    font1->drawText (xs, ys --, -3, "F1..7\tCAMERAS");
    font1->drawText (xs, ys --, -3, "F8\tSHOW MAP");
    font1->drawText (xs, ys --, -3, "ESC\tMAIN MENU");
    font1->drawText (xs, ys --, -3, "SPACE\tFIRE CANNON");
    font1->drawText (xs, ys --, -3, "ENTER\tFIRE MISSILE");
    font1->drawText (xs, (-- ys) --, -3, "MOUSE:");
    font1->drawText (xs, ys --, -3, "LEFT B.\tFIRE CANNON");
    font1->drawText (xs, ys --, -3, "RIGHT B.\tFIRE MISSILE");
    font1->drawText (xs, (-- ys) --, -3, "JOYSTICK:");
    font1->drawText (xs, ys --, -3, "BUTTON1\tFIRE CANNON");
    font1->drawText (xs, ys --, -3, "BUTTON2\tTARGET NEAREST ENEMY");
    font1->drawText (xs, ys --, -3, "BUTTON3\tFIRE MISSILE");
    font1->drawText (xs, ys --, -3, "BUTTON4\tSWITCH WEAPON");
    font1->drawText (xs, ys --, -3, "COOLIE\tNEXT/PREV TARGET");
  }

  drawMouseCursor ();
}

void pause_display ()
{
//  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();
//  glShadeModel(GL_SMOOTH);
  glPushMatrix ();

  CColor color (255, 255, 255, 255);
  font1->drawText (-3, -1, -1, "PAUSED", &color);

  glPopMatrix ();
//  glutSwapBuffers();
}

void credits_display ()
{
  glTranslatef (0, -3.5 + 0.015 * (float) creditstimer, 0);
  CColor col (255, 255, 255, 255);
  CColor col2 (255, 255, 0, 255);
  font2->drawTextCentered (0, 10, -2, "GAME PROGRAMMING,", &col);
  font2->drawTextCentered (0, 8, -2, "GRAPHICS, SOUND & MUSIC", &col);
  font1->drawTextCentered (0, 6, -2, "THOMAS A. DREXL", &col2);
  font2->drawTextCentered (0, 2, -2, "INTRO & MOON", &col);
  font1->drawTextCentered (0, 0, -2, "NORBERT DREXL", &col2);
// Maybe later :)
//  font2->drawTextCentered (0, -4, -2, "LENS FLARES & DEBUGGING", &col);
//  font1->drawTextCentered (0, -6, -2, "PIOTR PAWLOW", &col2);
}

void finish_display ()
{
  glTranslatef (0, -3.5 + 0.01 * (float) finishtimer, 0);
  CColor col (255, 255, 255, 255);
  font1->drawTextCentered (0, 12, -3, "CONGRATULATIONS!", &col);
  font1->drawTextCentered (0, 10, -3, "THE WORLD HAS BEEN SAVED YET AGAIN.", &col);
  font1->drawTextCentered (0, 6, -3, "THANK YOU FOR PLAYING GL-117.", &col);
  font1->drawTextCentered (0, 4, -3, "THIS GAME HAS ORIGINALLY BEEN DEVELOPED AS PART", &col);
  font1->drawTextCentered (0, 2, -3, "OF THE COURSE \"APPLICATIONS OF COMPUTER GRAPHICS\"", &col);
  font1->drawTextCentered (0, 0, -3, "AT THE TECHNICAL UNIVERSITY OF MUNICH, GERMANY,", &col);
  font1->drawTextCentered (0, -2, -3, "BY THOMAS ALEXANDER DREXL.", &col);
  font1->drawTextCentered (0, -6, -3, "THANKS TO HOOPY, THE GREATEST UNIX FREAK OUT THERE,", &col);
  font1->drawTextCentered (0, -8, -3, "JOSEF FOR HIS HELP TO DESIGN THE FIGHTER MODELS,", &col);
  font1->drawTextCentered (0, -10, -3, "PIOTR FOR THE LENS FLARES AND BERND FOR HIS IDEAS.", &col);
}

int vibration = 0;

void game_display ()
{
  int i;
  double sunx, suny, sunz;

  if (debug)
    printf ("\nentering myDisplayFunc()"); fflush (stdout);

  if (dithering) glEnable(GL_DITHER); else glDisable(GL_DITHER);

  bool sunvisible = false;
  float pseudoview = getView ();

  float mycamtheta = camtheta, mycamphi = camphi, mycamgamma = camgamma;

  if (vibration > 0)
  {
    if (camera == 0)
    {
      float sinvib = sin ((float) vibration);
      mycamphi += 0.2 * vibration * sinvib;
      mycamgamma += 0.2 * vibration * sinvib;
    }
    vibration --;
  }

//  if (fplayer->tl->y > l->getRayHeight (fplayer->tl->x, fplayer->tl->z))
//    sunblinding = true;

// calculate light factor
  if (camera == 0 && sunblinding && day && weather == WEATHER_SUNNY)
  {
    float np = fplayer->phi;
    if (np >= 180) np -= 360;
    sunlight = fabs (np) + fabs (fplayer->gamma - 180 - sungamma);
//  printf ("\nphi=%f, gamma=%f", fplayer->phi, fplayer->gamma);
    if (sunlight < 90)
    { sunlight = (90 - sunlight) / 20;
      if (sunlight < 1.0) sunlight = 1.0;
    /*printf ("\nlight=%f", sunlight); fflush (stdout);*/ }
    else sunlight = 1.0;
  }
  else
  {
    if (day) sunlight = 1.0;
    else sunlight = 0.75;
    if (flash)
    {
      sunlight = flash;
    }
  }


  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//  glClear(GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();

  glShadeModel(GL_SMOOTH);


  glPushMatrix ();
  
  if (camera != 1 && camera != 5)
    glRotatef (-mycamtheta, 0.0, 0.0, 1.0);
/*  if (camera == 0)
    glRotatef (-fplayer->gamma + 180, 1.0, 0.0, 0.0);
  else*/
    glRotatef (mycamgamma, 1.0, 0.0, 0.0);
  glRotatef (-mycamphi, 0.0, 1.0, 0.0);
//  if (camera == 0)
//    glRotatef (-20, 1.0, 0.0, 0.0);

  // draw sky
  glDisable (GL_FOG);
  glDisable (GL_DEPTH_TEST);

  float mylight = sunlight;
  if (!day) mylight /= 0.75;
  if (mylight > 1.0 && day)
    mylight = mylight / 20.0 + 0.95;
  else if (mylight > 1.0 && !day)
    mylight = mylight / 5.0 + 0.8;
  gl->foglum = mylight;
  sphere->drawGL (tlminf, tlinf, tlnull, space->alpha, mylight, true, false);

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
      glRotatef (/*-camphi +*/ star [i]->phi, 0.0, 1.0, 0.0);
/*      if (camera == 0)
        glRotatef (star [i]->gamma, 1.0, 0.0, 0.0);
      else*/
        glRotatef (star [i]->gamma, 1.0, 0.0, 0.0);
      glTranslatef (0, 0, -10);
      star [i]->draw ();
      glPopMatrix ();
    }
  }
  }

//  sphere.draw (tlnull, tlnull, , 1.0);
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_FOG);
//  glPopMatrix ();


// draw sun or moon (or earth)
//  glPushMatrix ();
  float r = 10.0;
//  float p = -fplayer->phi, t = -fplayer->gamma;
  float fac = view, zfac = view * 0.2;
//  if (camera != 1 && camera != 5)
//    glRotatef (-camtheta, 0.0, 0.0, 1.0);
  if (weather == WEATHER_SUNNY || weather == WEATHER_CLOUDY)
  {
  if (camera == 0)
    glRotatef (/*-fplayer->gamma + 180 +*/ sungamma, 1.0, 0.0, 0.0);
  else
    glRotatef (mycamgamma + sungamma, 1.0, 0.0, 0.0);
//  glRotatef (-camphi, 0.0, 1.0, 0.0);
  float zf = -11;
  if (day) zf = -12;
  if (l->type == LAND_MOON && !day) zf = -8; // diplay bigger earth
  glTranslatef (0, 0, zf);
/*  if (camera == 0)
    glRotatef (-fplayer->gamma + 180 + sungamma, 1.0, 0.0, 0.0);
  else
    glRotatef (camgamma + sungamma, 1.0, 0.0, 0.0);
  glRotatef (-camphi, 0.0, 1.0, 0.0);*/
  gl->extractFrustum ();
  if (gl->isPointInFrustum (-1, 1, 0) || gl->isPointInFrustum (-1, -1, 0) ||
      gl->isPointInFrustum (1, -1, 0) || gl->isPointInFrustum (1, 1, 0))
  {
    sunvisible = true;
    glDisable (GL_DEPTH_TEST);
    if (day) gl->enableTextures (texsun->textureID);
    else if (l->type != LAND_MOON) gl->enableTextures (texmoon->textureID);
    else gl->enableTextures (texearth->textureID);
    if (day && l->type != 1)
      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl->enableAlphaBlending ();
    glEnable (GL_ALPHA_TEST);
    float alphamax = 0.1;
    if (day) alphamax = 0.9;
    glAlphaFunc (GL_GEQUAL, alphamax);
  //  glDisable (GL_DITHER);
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
  //  glColor4f (0.1, 0.9, 0.9, 1.0);
    glTexCoord2d (1, 0);
    glVertex3f (1, -1, 0);
    glTexCoord2d (0, 0);
    glVertex3f (-1, -1, 0);
    glEnd ();
//    if (day && l->type != 1)
//      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  /* Where is the sun? */
  double proj[16];
  double modl[16];
  int vp[4];
  glGetDoublev( GL_PROJECTION_MATRIX, proj );
  glGetDoublev( GL_MODELVIEW_MATRIX, modl );
  glGetIntegerv( GL_VIEWPORT, vp );
  gluProject (0, 0, 0, modl, proj, vp, &sunx, &suny, &sunz);

    glDisable (GL_ALPHA_TEST);
    glEnable (GL_DEPTH_TEST);
    glDisable (GL_TEXTURE_2D);
    gl->disableAlphaBlending ();
  }
  }

  glPopMatrix ();
  
    
  glPushMatrix ();

  /* Modeltransformation ausfuehren... */

  if (camera != 1 && camera != 5)
    glRotatef (-mycamtheta, 0.0, 0.0, 1.0);

/*  if (camera == 0)
    glRotatef (-fplayer->gamma + 180, 1.0, 0.0, 0.0);
  else*/
    glRotatef (mycamgamma, 1.0, 0.0, 0.0);

  glDisable (GL_FOG);
/*  glPushMatrix ();
  glDisable (GL_DEPTH_TEST);
  glBegin (GL_QUAD_STRIP);
  float fac = view, zfac = view * 0.2;
  glColor4f (0.0, 1.0, 1.0, 1.0);
  glVertex3f (-fac, 2.0*fac, -zfac);
  glVertex3f (fac, 2.0*fac, -zfac);
  glColor4f (0.1, 0.9, 0.9, 1.0);
  glVertex3f (-fac, fac*0.4, -zfac);
  glVertex3f (fac, fac*0.4, -zfac);
  glColor4f (0.5, 0.5, 0.5, 1.0);
  glVertex3f (-fac, -fac*0.125, -zfac);
  glVertex3f (fac, -fac*0.125, -zfac);
  glVertex3f (-fac, -2.0*fac, -zfac);
  glVertex3f (fac, -2.0*fac, -zfac);
  glEnd ();
  glEnable (GL_DEPTH_TEST);
  
  glPopMatrix ();*/
//  glEnable (GL_FOG);


  glRotatef (-mycamphi, 0.0, 1.0, 0.0);  /* Rotation um die Y-Achse */


    

//  glPushMatrix ();
//  if (camera == 0)
//    glRotatef (-20, 1.0, 0.0, 0.0);
  glScalef (1, 1, 1);








  /*glPushMatrix ();
  if (camera == 0)
    glRotatef (-20, 1.0, 0.0, 0.0);
  glDisable (GL_DEPTH_TEST);

  drawTexture (10.0, -fplayer->phi - 20, 20 + 30, -fplayer->phi + 20, - 20 + 30);

  glEnable (GL_DEPTH_TEST);
  gl->disableAlphaBlending ();
  glPopMatrix ();*/
  
  
//  fplayer->tl->y = -(0.6*zoom - (float)l->h[(int)camx+MAXX/2][MAXX/2-(int)camz]*zoomz * zoom);
//  if (camera != 50)
//    camy = fplayer->tl->y + fplayer->zoom / 3.0;
//  if (camera == 1 || camera == 2) camy += fplayer->zoom * 0.6;
  glTranslatef (-camx /*-2*px / MAXX * zoom*/, -camy /*0.62*zoom - (float)l->h[(int)px+MAXX/2][MAXX/2-(int)pz]*zoomz * zoom*/, -camz/*-2*pz / MAXX * zoom*/);    /* Verschiebung um -3 in z-Richtung */

//  glPushMatrix();









/*  if (lighting)
  {
    glEnable( GL_LIGHTING);
    glEnable( GL_LIGHT0 );
  }*/

  if (camera != 50)
  {
    gl->enableFog (pseudoview);
  }

//  glEnable (GL_DITHER);

// draw terrain
  l->calcDynamicLight (explosion, laser, (DynamicObj **) missile);
  glEnable (GL_CULL_FACE);
  l->draw ((int) mycamphi, (int) (-mycamgamma + 180.0));
  glDisable (GL_CULL_FACE);

  gl->extractFrustum ();
//  gl->enableTextures (texgrass);
//  glEnable (GL_CULL_FACE);
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
    if (flash > 5)
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
      space->drawGL ();
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
//  glDisable (GL_CULL_FACE);
  glDisable (GL_TEXTURE_2D);

  glDisable (GL_LIGHTING);

  if (camera == 0)
  {
    glDisable (GL_DEPTH_TEST);
    cockpit->drawTargeter ();
    glEnable (GL_DEPTH_TEST);
  }

  glPopMatrix ();

//  gl->enableAntiAliasing ();

  /* Where is the sun? */
/* double ra=10.0;
  double winx, winy, winz;
  winx = ra*sin(fplayer->gamma)*cos(fplayer->phi)*width;
  winy = ra*sin(fplayer->gamma)*sin(fplayer->phi)*height;
  winz = ra*cos(fplayer->gamma);
  double proj[16];
  double modl[16];
  int vp[4];
  glGetDoublev( GL_PROJECTION_MATRIX, proj );
  glGetDoublev( GL_MODELVIEW_MATRIX, modl );
  vp[0]=0; vp[1]=0; vp[2]=width; vp[3]=height;
//  gluProject (0, 0, 1.0, proj, modl, vp, &winx, &winy, &winz);
  double objx, objy, objz;
  gluUnProject (winx, winy, winz, modl, proj, vp, &objx, &objy, &objz);
  printf ("\nre: x=%f, y=%f, z=%f", objx, objy, objz); fflush (stdout);
  glBegin (GL_LINES);
  glVertex3f (-objx, -objy, objz);
  glVertex3f (-objx, -objy, 5);
  glEnd ();*/

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
  while (CTexture *tex=fl_texture[i]) {
  double position=fl_position[i];
  double flarex,flarey,size=fl_size[i];

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

  if (sunvisible) {
    GLfloat zbuf[1];
    glReadPixels((int)sunx,(int)suny,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,zbuf);
    sunblinding=(zbuf[0]<1)?false:true;
  } else sunblinding=false;

/*  int maxi = 4;
  for (i = maxi - 1; i >= 1; i --)
  {
  glDisable (GL_DEPTH_TEST);
  glPushMatrix ();
  r = 10.0;
  fac = view; zfac = view * 0.2;
  int myphi = (int) mycamphi;
  if (camera != 1 && camera != 5)
  glRotatef (-mycamtheta, 0.0, 0.0, 1.0);
  glRotatef (mycamgamma + sungamma * i / maxi, 1.0, 0.0, 0.0);
  if (myphi < 180) myphi = myphi * i / maxi;
  else myphi = 360 - (360 - myphi) * i / maxi;
  glRotatef (-myphi, 0.0, 1.0, 0.0);
  glTranslatef (0, 0, (float) -10.0 * i / maxi);
  glRotatef (mycamgamma + sungamma * i / maxi, 1.0, 0.0, 0.0);
  glRotatef (-myphi, 0.0, 1.0, 0.0);
  glDisable (GL_DEPTH_TEST);
  gl->enableTextures (texflare->textureID);
  gl->enableAlphaBlending ();
//  glDisable (GL_DITHER);
  glBegin (GL_QUADS);
  fac = view; zfac = view * 3.5;
  glColor4f (1.0, 1.0, 1.0, 1.0);
  glTexCoord2d (0, 1);
  glVertex3f (-0.5, 0.5, 0);
  glTexCoord2d (1, 1);
  glVertex3f (0.5, 0.5, 0);
//  glColor4f (0.1, 0.9, 0.9, 1.0);
  glTexCoord2d (1, 0);
  glVertex3f (0.5, -0.5, 0);
  glTexCoord2d (0, 0);
  glVertex3f (-0.5, -0.5, 0);
  glEnd ();
  glEnable (GL_DEPTH_TEST);
  glDisable (GL_TEXTURE_2D);
  gl->disableAlphaBlending ();
  glPopMatrix ();
  }
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }  
*/

//  float zf, g, g2;
//  glPushMatrix ();
  if (camera == 0)
  {
    cockpit->drawCross ();
    cockpit->drawHeading ();
    cockpit->drawRadar ();
    cockpit->drawTargetedElement ();
    cockpit->drawWeapon ();
  }
//  glPopMatrix ();
//  glutStrokeCharacter (GLUT_STROKE_ROMAN, 'A');

/*  drawText (-5, 0, -10, "DOGFIGHT");
  drawText (-5, -3, -10, "OPTIONS");*/

  mission->draw ();

  glPushMatrix ();
  glDisable (GL_DEPTH_TEST);
/*  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glLoadIdentity ();
  gluOrtho2D (0, width, 0, height);
  glScalef (1, -1, 1);
  glTranslatef (0, -height, 0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glColor3d (255, 255, 0);
  char *c;
  char *string = "action fs demo";
  glRasterPos2f (20, 25);
  for (c = string; *c; c ++)
  {
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, *c);
//    glutStrokeCharacter (GLUT_STROKE_ROMAN, *c);
  }
  char buf [25];
  sprintf (buf, "FPS: %d", (int) fps);
  glRasterPos2f (20, 50);
  for (c = buf; *c; c ++)
  {
    glutBitmapCharacter (GLUT_BITMAP_8_BY_13, *c);
  }

  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();
  glMatrixMode (GL_MODELVIEW);*/
  glPopMatrix ();

  char buf [25];
  CColor col;
  if (fps < 28)
  {
    sprintf (buf, "FPS: %d", (int) fps);
    font1->drawText (-25, 25, -3.5, buf, &col);
  }
  else
    font1->drawText (-25, 25, -3.5, "FPS: PERFECT", &col);
  if (fps <= 20)
  {
    CColor colorred (255, 0, 0, 255);
    font1->drawTextCentered (0, -8, -2, "FPS TOO LOW", &colorred);
    font1->drawTextCentered (0, -9, -2, "TURN DOWN VIEW OR QUALITY", &colorred);
  }

/*  if (missionstate == 1)
  {
    if (fplayer->score == -1)
    {
      fplayer->score = mission->getScore (missionstate);
    }
    CColor colblue (0, 0, 255, 200);
    font1->drawTextCentered (0, 1, -1.2, "MISSION", &colblue);
    font1->drawTextCentered (0, -1, -1.2, "SUCCESS", &colblue);
    sprintf (buf, "SCORE: %d", fplayer->score);
    font1->drawTextCentered (0, -4, -2, buf, &colblue);
  }
  else if (missionstate == 2)
  {
    if (fplayer->score == -1)
    {
      fplayer->score = mission->getScore (missionstate);
    }
    CColor colred (255, 0, 0, 200);
    font1->drawTextCentered (0, 1, -1.2, "MISSION", &colred);
    font1->drawTextCentered (0, -1, -1.2, "FAILED", &colred);
    sprintf (buf, "SCORE: %d", fplayer->score);
    font1->drawTextCentered (0, -4, -2, buf, &colred);
  }*/

  if (controls == CONTROLS_MOUSE)
    drawMouseCursor ();

  if (debug)
    printf ("\nleaving myDisplayFunc()"); fflush (stdout);
}

float lastspeed;
int gametimer;

void game_timer ()
{
  if (multiplayer) return;

  int i, i2;

  gametimer ++;

  if (fplayer->autofire)
  {
    fplayer->fireCannon (laser);
    sound->play (SOUND_CANNON1);
  }

  if (lastshield > fplayer->shield && !fplayer->ai)
  {
    sound->play (SOUND_HIT1);
    lastshield = fplayer->shield;
    vibration = 25;
  }

  if (debug)
    printf ("\nentering myTimerFunc()"); fflush (stdout);

    if (weather == WEATHER_THUNDERSTORM && !flash && !myrandom (40))
    {
      flash = 10;
      int fphi = (int) camphi + myrandom (50) - 25;
      if (fphi < 0) fphi += 360;
      else if (fphi >= 360) fphi -= 360;
      float pseudoview = getView ();
      float fdist = myrandom ((int) pseudoview - 20) + 10;
      float fx = fplayer->tl->x - sine [fphi] * fdist;
      float fz = fplayer->tl->z - cosi [fphi] * fdist;
      flash1->set (fx, l->getHeight (fx, fz), fz, camphi);
      int lev = (int) (128.0 - 80.0 * fdist / (pseudoview - 10));
      sound->setVolume (SOUND_THUNDER1, lev);
      sound->play (SOUND_THUNDER1);
    }

  if (initing) return;

  if (flash)
    flash --;
  if (flash <= 5 && flash > 0)
    flash --;

  if (lastspeed != fplayer->speed && !(gametimer & 15))
    setPlaneVolume ();
  lastspeed = fplayer->speed;

  for (i = 0; i < maxfighter; i ++)
  {
    for (i2 = 0; i2 < maxlaser; i2 ++)
      if (laser [i2]->active)
        fighter [i]->collide (laser [i2]);
    for (i2 = 0; i2 < maxmissile; i2 ++)
      if (missile [i2]->active)
        fighter [i]->collide (missile [i2]);
    for (i2 = 0; i2 < maxfighter; i2 ++)
      if (fighter [i2]->active)
        if (i != i2)
          fighter [i]->collide (fighter [i2]);
  }

  for (i = 0; i < maxfighter; i ++)
  {
    fighter [i]->aiAction ((AIObj **) fighter, missile, laser);
    float lev;
    if (fighter [i]->explode == 3 && (lev = fplayer->distance (fighter [i])) < 32)
    {
      sound->setVolume (SOUND_EXPLOSION1, 128 - (int) (lev * 4.0));
      sound->play (SOUND_EXPLOSION1);
    }
  }
  for (i = 0; i < maxlaser; i ++)
  {
    laser [i]->move ();
  }
  for (i = 0; i < maxmissile; i ++)
  {
    missile [i]->aiAction ((AIObj **) fighter, missile, laser);
  }

  for (i = 0; i < maxexplosion; i ++)
    explosion [i]->move ();
  for (i = 0; i < maxblacksmoke; i ++)
    blacksmoke [i]->move ();

  if (fplayer->shield <= 0)
    camera = 1;
/*  if (!fplayer->active && !fplayer->draw)
    game_levelInit ();*/

  float cf = -fplayer->zoom / 2;
  camtheta = fplayer->theta;
  if (camera == 0)  // cockpit
  {
    camx = fplayer->tl->x + cf * sine [(int) fplayer->phi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + fplayer->zoom / 3.0;
    camz = fplayer->tl->z + cf * cosi [(int) fplayer->phi];// * MAXX / zoom / 2;
    camphi = fplayer->phi;
    camgamma = -fplayer->gamma + 180;
    fplayer->draw = 0;
  }
  if (camera == 1) // chase
  {
    cf = fplayer->zoom * 2;
    camx = fplayer->tl->x + cf * sine [(int) fplayer->phi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * cosi [(int) fplayer->phi];// * MAXX / zoom / 2;
    camphi = fplayer->phi;
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 2) // backwards
  {
    cf = -fplayer->zoom * 2;
    camx = fplayer->tl->x + cf * sine [(int) fplayer->phi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * cosi [(int) fplayer->phi];// * MAXX / zoom / 2;
    camphi = fplayer->phi + 180.0;
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 3) // other players
  {
    cf = fighter [aktcam]->zoom * 2;
    camx = fighter [aktcam]->tl->x + cf * sine [(int) fighter [aktcam]->phi];// * MAXX / zoom / 2;
    camy = fighter [aktcam]->tl->y + fighter [aktcam]->zoom;
    camz = fighter [aktcam]->tl->z + cf * cosi [(int) fighter [aktcam]->phi];// * MAXX / zoom / 2;
    camphi = fighter [aktcam]->phi;
    camgamma = 20;
    camtheta = fighter [aktcam]->theta;
    fplayer->draw = 1;
  }
  else if (camera == 4) // missile
  {
    cf = missile [0]->zoom * 5;
    camx = missile [0]->tl->x + cf * sine [(int) missile [0]->phi];// * MAXX / zoom / 2;
    camy = missile [0]->tl->y + fplayer->zoom;
    camz = missile [0]->tl->z + cf * cosi [(int) missile [0]->phi];// * MAXX / zoom / 2;
    camphi = missile [0]->phi;
    fplayer->draw = 1;
  }
  else if (camera == 5) // top
  {
    cf = fplayer->zoom * 15;
    camx = fplayer->tl->x + cf * sine [(int) fplayer->phi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + 5.5;
    camz = fplayer->tl->z + cf * cosi [(int) fplayer->phi];// * MAXX / zoom / 2;
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
    camx = fplayer->tl->x + cf * sine [(int) camphi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * cosi [(int) camphi];// * MAXX / zoom / 2;
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 7) // right
  {
    cf = fplayer->zoom * 2;
    camphi = fplayer->phi + 270.0;
    if (camphi >= 360) camphi -= 360;
    else if (camphi < 0) camphi += 360;
    camx = fplayer->tl->x + cf * sine [(int) camphi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * cosi [(int) camphi];// * MAXX / zoom / 2;
    fplayer->draw = 1;
    camgamma = 20;
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

//  printf ("%f, %f\n", fplayer->tl->x, fplayer->tl->z);
  missionstate = mission->processtimer ();

  if (missionstate == 2)
  {
    missionending ++;
    if (missionending >= 25)
    {
      missionending = 0;
      if (mission->difficulty == 0) fplayer->points -= 25;
      else if (mission->difficulty == 1) fplayer->points += 25;
      else if (mission->difficulty == 2) fplayer->points += 75;
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
      if (mission->difficulty == 0) fplayer->points -= 25;
      else if (mission->difficulty == 1) fplayer->points += 25;
      else if (mission->difficulty == 2) fplayer->points += 75;
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

  if (debug)
    printf ("\nleaving myTimerFunc()"); fflush (stdout);
}

int net_thread_main (void *data)
{
  while (true)
  {

  int i, i2;

  gametimer ++;

  if (debug)
    printf ("\nentering myTimerFunc()"); fflush (stdout);

    if (weather == WEATHER_THUNDERSTORM && !flash && !myrandom (30))
    {
      flash = 10;
      int fphi = (int) camphi + myrandom (50) - 25;
      if (fphi < 0) fphi += 360;
      else if (fphi >= 360) fphi -= 360;
      float pseudoview = getView ();
      float fdist = myrandom ((int) pseudoview - 20) + 10;
      float fx = fplayer->tl->x - sine [fphi] * fdist;
      float fz = fplayer->tl->z - cosi [fphi] * fdist;
      flash1->set (fx, l->getHeight (fx, fz), fz, camphi);
      int lev = (int) (128.0 - 80.0 * fdist / (pseudoview - 10));
      sound->setVolume (SOUND_THUNDER1, lev);
      sound->play (SOUND_THUNDER1);
    }

  if (flash)
    flash --;
  if (flash <= 5 && flash > 0)
    flash --;

  if (lastspeed != fplayer->speed && !(gametimer & 15))
    setPlaneVolume ();
  lastspeed = fplayer->speed;

  for (i = 0; i < maxfighter; i ++)
  {
    for (i2 = 0; i2 < maxlaser; i2 ++)
      if (laser [i2]->active)
        fighter [i]->collide (laser [i2]);
    for (i2 = 0; i2 < maxmissile; i2 ++)
      if (missile [i2]->active)
        fighter [i]->collide (missile [i2]);
    for (i2 = 0; i2 < maxfighter; i2 ++)
      if (fighter [i2]->active)
        if (i != i2)
          fighter [i]->collide (fighter [i2]);
  }

  for (i = 0; i < maxfighter; i ++)
  {
    fighter [i]->aiAction ((AIObj **) fighter, missile, laser);
  }
  for (i = 0; i < maxlaser; i ++)
    laser [i]->move ();
  for (i = 0; i < maxmissile; i ++)
    missile [i]->aiAction ((AIObj **) fighter, missile, laser);

  for (i = 0; i < maxexplosion; i ++)
    explosion [i]->move ();
  for (i = 0; i < maxblacksmoke; i ++)
    blacksmoke [i]->move ();

  if (fplayer->shield <= 0)
    camera = 1;
/*  if (!fplayer->active && !fplayer->draw)
    game_levelInit ();*/

  float cf = -fplayer->zoom / 2;
  camtheta = fplayer->theta;
  if (camera == 0)  // cockpit
  {
    camx = fplayer->tl->x + cf * sine [(int) fplayer->phi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + fplayer->zoom / 3.0;
    camz = fplayer->tl->z + cf * cosi [(int) fplayer->phi];// * MAXX / zoom / 2;
    camphi = fplayer->phi;
    fplayer->draw = 0;
  }
  if (camera == 1) // chase
  {
    cf = fplayer->zoom * 2;
    camx = fplayer->tl->x + cf * sine [(int) fplayer->phi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * cosi [(int) fplayer->phi];// * MAXX / zoom / 2;
    camphi = fplayer->phi;
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 2) // backwards
  {
    cf = -fplayer->zoom * 2;
    camx = fplayer->tl->x + cf * sine [(int) fplayer->phi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * cosi [(int) fplayer->phi];// * MAXX / zoom / 2;
    camphi = fplayer->phi + 180.0;
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 3) // other players
  {
    cf = fighter [aktcam]->zoom * 2;
    camx = fighter [aktcam]->tl->x + cf * sine [(int) fighter [aktcam]->phi];// * MAXX / zoom / 2;
    camy = fighter [aktcam]->tl->y + fighter [aktcam]->zoom;
    camz = fighter [aktcam]->tl->z + cf * cosi [(int) fighter [aktcam]->phi];// * MAXX / zoom / 2;
    camphi = fighter [aktcam]->phi;
    camgamma = 20;
    camtheta = fighter [aktcam]->theta;
    fplayer->draw = 1;
  }
  else if (camera == 4) // missile
  {
    cf = missile [0]->zoom * 5;
    camx = missile [0]->tl->x + cf * sine [(int) missile [0]->phi];// * MAXX / zoom / 2;
    camy = missile [0]->tl->y + fplayer->zoom;
    camz = missile [0]->tl->z + cf * cosi [(int) missile [0]->phi];// * MAXX / zoom / 2;
    camphi = missile [0]->phi;
    fplayer->draw = 1;
  }
  else if (camera == 5) // top
  {
    cf = fplayer->zoom * 15;
    camx = fplayer->tl->x + cf * sine [(int) fplayer->phi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + 5.5;
    camz = fplayer->tl->z + cf * cosi [(int) fplayer->phi];// * MAXX / zoom / 2;
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
    camx = fplayer->tl->x + cf * sine [(int) camphi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * cosi [(int) camphi];// * MAXX / zoom / 2;
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 7) // right
  {
    cf = fplayer->zoom * 2;
    camphi = fplayer->phi + 270.0;
    if (camphi >= 360) camphi -= 360;
    else if (camphi < 0) camphi += 360;
    camx = fplayer->tl->x + cf * sine [(int) camphi];// * MAXX / zoom / 2;
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * cosi [(int) camphi];// * MAXX / zoom / 2;
    fplayer->draw = 1;
    camgamma = 20;
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

//  printf ("%f, %f\n", fplayer->tl->x, fplayer->tl->z);
  missionstate = mission->processtimer ();

  if (missionstate == 2)
  {
    missionending ++;
    if (missionending >= 35)
    {
      missionending = 0;
      printf ("initing new mission"); fflush (stdout);
      missionactive = false;
      createMission (MISSION_DEMO);
      game_levelInit ();
      switch_menu ();
    }
  }

  if (missionstate == 1)
  {
    missionending ++;
    if (missionending >= 35)
    {
      missionending = 0;
      printf ("initing new mission"); fflush (stdout);
      missionactive = false;
      createMission (MISSION_DEMO);
      game_levelInit ();
      switch_menu ();
    }
  }

  if (debug)
    printf ("\nleaving myTimerFunc()"); fflush (stdout);


  if (multiplayer)
  {

    if (isserver)
    {
      server->sendMessage (1, ":", 1);
      int len = fplayer->net_write ();
      server->sendMessage (1, fplayer->net, len);
      len = fighter[1]->net_write ();
      server->sendMessage (1, fighter[1]->net, len);
//      while (server->getMessage (1, fighter [1]->net))
//        fighter [1]->net_read ();
    }
    else
    {
//      int len = fplayer->net_write ();
//      client->sendMessage (fplayer->net, len);
      char buffer[10];
      while (! client->getMessage(buffer));
      while (!client->getMessage (fighter [0]->net)) ;
      fighter [0]->net_read ();
      while (!client->getMessage (fighter [1]->net)) ;
      fighter [1]->net_read ();
      // if (client)
    }

  }

#ifdef HAVE_SDL
    SDL_Delay (50);
#endif
  }
}

int starttime;
float lastfps = -1;

void menu_timer ()
{
  menutimer ++;
  if (!(menutimer & 0xFF))
  {
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
    if (akttime - starttime < 10000)
      if (lastfps != fps)
      {
        lastfps = fps;
        if (fps >= 29)
        {
          if (view <= 40) view += 10; // view 50 erreichen
          else if (quality <= 3) quality ++; // quality 4 erreichen
          else if (view <= 60) view += 10; // view 70 erreichen
          else if (quality <= 4) quality ++; // quality 5 erreichen
          else if (view <= 90) view += 10; // view 100 erreichen
        }
        else if (fps <= 23)
        {
          if (quality >= 5) quality --; // quality 4 erreichen
          else if (view >= 60) view -= 10; // view 50 erreichen
          else if (quality >= 3) quality --; // quality 2 erreichen
          else if (view >= 50) view -= 10; // view 40 erreichen
          else if (quality >= 1) quality --; // quality 0 erreichen
          else if (view >= 30) view -= 10; // view 20 erreichen
        }
        menu_reshape ();
      }
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void stats_timer ()
{
  menutimer ++;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void mission_timer ()
{
  missionmenutimer ++;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void credits_timer ()
{
  creditstimer ++;
  if (creditstimer > 360)
    creditstimer = 0;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void finish_timer ()
{
  finishtimer ++;
  if (finishtimer > 800)
    finishtimer = 0;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}
//#include <time.h>

void create_key (unsigned char key, int x, int y)
{
  if (key == 's')
  {
    createMission (MISSION_MULTIPLAYER_DOGFIGHT);
    game_levelInit ();
    switch_game ();
    missionactive = true;
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
}

void create_timer ()
{
#ifndef USE_GLUT
//  server->createSocketSet ();
  server->getClient ();
  SDL_Delay (100);
#endif
}

void join_timer ()
{
#ifndef USE_GLUT
  client->getServer ("Lara", "client1");
  SDL_Delay (100);
#endif
}

void myInit ()
{
  int i;
  if (debug)
    printf ("\nentering myInit()"); fflush (stdout);

  if (debug)
    printf ("\n initing landscape"); fflush (stdout);

  texsun = gl->genTextureTGA (dirs->getTextures ("sun2.tga"), 1, 2, 0);
  texmoon = gl->genTextureTGA (dirs->getTextures ("moon1.tga"), 1, 2, 0);
  texearth = gl->genTextureTGA (dirs->getTextures ("earth.tga"), 1, 0, 0);
//  texflare = gl->genTextureTGA (dirs->getTextures ("flare1.tga"), 1, 2, 0);
  texflare1 = gl->genTextureTGA (dirs->getTextures ("flare1.tga"), 1, -1, 0);
  texflare2 = gl->genTextureTGA (dirs->getTextures ("flare2.tga"), 1, -1, 0);
  texflare3 = gl->genTextureTGA (dirs->getTextures ("flare3.tga"), 1, -1, 0);
  texflare4 = gl->genTextureTGA (dirs->getTextures ("flare4.tga"), 1, -1, 0);
  texgrass = gl->genTextureTGA (dirs->getTextures ("grass1.tga"), 0, 0, 1);
  texrocks = gl->genTextureTGA (dirs->getTextures ("rocks1.tga"), 0, 0, 1);
  texwater = gl->genTextureTGA (dirs->getTextures ("water1.tga"), 0, 0, 1);
  texsand = gl->genTextureTGA (dirs->getTextures ("sand1.tga"), 0, 0, 1);
  texredstone = gl->genTextureTGA (dirs->getTextures ("redstone2.tga"), 0, 0, 1);
  textree = gl->genTextureTGA (dirs->getTextures ("tree1.tga"), 0, 3, 1);
  textree2 = gl->genTextureTGA (dirs->getTextures ("tree2.tga"), 0, 3, 1);
  textree3 = gl->genTextureTGA (dirs->getTextures ("tree3.tga"), 0, 3, 1);
  textree4 = gl->genTextureTGA (dirs->getTextures ("tree4.tga"), 0, 3, 1);
  texcactus1 = gl->genTextureTGA (dirs->getTextures ("cactus1.tga"), 0, 3, 1);
  texsmoke = gl->genTextureTGA (dirs->getTextures ("smoke1.tga"), 0, 4, 1);
  texsmoke2 = gl->genTextureTGA (dirs->getTextures ("smoke2.tga"), 0, 4, 1);
  texsmoke3 = gl->genTextureTGA (dirs->getTextures ("smoke2.tga"), 0, 5, 1);
  texcross = gl->genTextureTGA (dirs->getTextures ("cross.tga"), 0, 2, 1);
//  texcross2 = gl->genTextureTGA (dirs->getTextures ("cross2.tga"), 0, 2, 1);
  texcross2 = gl->genTextureTGA (dirs->getTextures ("cross2.tga"), 0, -1, 1);
  texranks = gl->genTextureTGA (dirs->getTextures ("ranks.tga"), 0, 0, 1);
  texmedals = gl->genTextureTGA (dirs->getTextures ("medals.tga"), 0, 0, 1);
//  texfont1 = gl->genTextureTGA ("textures/font1.tga", 0, 3, 0);
/*  for (i = 0; i < maxchars; i ++)
  {
    char buf [64] = "../textures/char0.tga";
    buf [16] = '1' + i;
    texchar [i] = gl->genTextureTGA (buf, 0, 3, 1);
  }*/

//  l = new GLLandscape (space);

  tlinf = new CVector3 (1E10, 1E10, 1E10);
  tlminf = new CVector3 (-1E10, -1E10, -1E10);
  tlnull = new CVector3 (0, 0, 0);

  if (debug)
    printf ("\n initing fighters"); fflush (stdout);

  for (i = 0; i < maxgroundobj; i ++)
  {
    groundobj [i] = new DynamicObj (space, &model_tent1, 3);
  }

/*  obj = new Model ();
  obj->loadFromFile ("./data/f4.v3d");*/

  explsphere = new CSphere ();
  ((CSphere *) explsphere)->init (1, 9);
  CColor explcolor (255, 255, 1);
  explsphere->setColor (&explcolor);
  for (i = 0; i < explsphere->object [0]->numVertices; i ++)
    explsphere->object [0]->vertex [i].color.setColor (myrandom (100) + 155, myrandom (100) + 100, 0);
//  ((CSphere *) explsphere)->invertNormals ();
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
  }

  if (debug)
    printf ("\n initing sphere"); fflush (stdout);

  objsphere = new CSphere (1, 9, 1, 1, 1);
  sphere = new CSpaceObj (objsphere, 10.0);
  //sphere = new CSpaceObj ();
  sphere->rot->b = 90;
  sphere->draw = 2;
  sphere->drawlight = false;

  flash1 = new Flash ();

  if (debug)
    printf ("\n initing lasers"); fflush (stdout);

//  objlaser = new CModel ();
//  objlaser->loadFromFile ("./data/c1.v3d");
  for (i = 0; i < maxlaser; i ++)
  {
    laser [i] = new DynamicObj (space, &model_cannon1, 0.07);
  }

  if (debug)
    printf ("\n initing missiles"); fflush (stdout);

//  objmissile = new CModel ();
//  objmissile->loadFromFile ("./data/r1.v3d");
  for (i = 0; i < maxmissile; i ++)
  {
    missile [i] = new AIObj (space, &model_missile1, 0.1);
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

  if (debug)
    printf ("\nleaving myInit()"); fflush (stdout);
}


CRotation rot;
CRotation rot2;
CVector3 tl;
CVector3 tl2;
int initexplode = 0;
int initexplode1 = 0;
int i;
int inittimer = 0;

void init_reshape ()
{
  /* Darstellung auf gesamten Clientbereich des Fensters zulassen */
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  /* angle, aspect, near Clip, far Clip */
  float v = view;
  if (camera == 50) v = 100000.0;
  gluPerspective (80.0, 1.0, nearclippingplane, 80); // should be sqrt(2) or 1.5
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
}

void myFirstInit ()
{
  int i;

  if (debug) { printf ("\niniting mathtab"); fflush (stdout); }
  mathtab_init ();

  if (debug) { printf ("\nnew GL"); fflush (stdout); }
  gl = new GL (); //(1.0, 1000.0);
  if (debug) { printf ("\nnew fonts"); fflush (stdout); }
  font1 = new Font (dirs->getTextures ("font1.tga"), 32, '!');
  font2 = new Font (dirs->getTextures ("font2.tga"), 32, '!');
//  texfont1 = gl->genTextureTGA ("../textures/font2.tga", 0, 1, 0);
  if (debug) { printf ("\nimporting models"); fflush (stdout); }
  g_Load3ds.Import3DS (&model_fig, dirs->getModels ("fig7.3ds"));
  model_fig.setName ("FALCON");
  if (debug) { printf ("\n1"); fflush (stdout); }
  g_Load3ds.Import3DS (&model_figa, dirs->getModels ("fig4.3ds"));
  model_figa.setName ("SWALLOW");
  if (debug) { printf ("\n1"); fflush (stdout); }
  g_Load3ds.Import3DS (&model_figb, dirs->getModels ("fig2.3ds"));
  model_figb.setName ("HAWK");
  if (debug) { printf ("\n1"); fflush (stdout); }
  g_Load3ds.Import3DS (&model_figc, dirs->getModels ("fig3.3ds"));
  model_figc.setName ("HAWK II");
  if (debug) { printf ("\n1"); fflush (stdout); }
  g_Load3ds.Import3DS (&model_figd, dirs->getModels ("fig1.3ds"));
  model_figd.setName ("BUZZARD");
  if (debug) { printf ("\n1"); fflush (stdout); }
  g_Load3ds.Import3DS (&model_fige, dirs->getModels ("fig6.3ds"));
  model_fige.setName ("CROW");
  if (debug) { printf ("\n1"); fflush (stdout); }
  g_Load3ds.Import3DS (&model_figf, dirs->getModels ("fig8.3ds"));
  model_figf.setName ("PHOENIX");
  g_Load3ds.Import3DS (&model_figg, dirs->getModels ("fig9.3ds"));
  model_figg.setName ("RED ARROW");
  g_Load3ds.Import3DS (&model_figh, dirs->getModels ("fig5.3ds"));
  model_figh.setName ("BLACKBIRD");
  g_Load3ds.Import3DS (&model_figt, dirs->getModels ("transp1.3ds"));
  model_figt.setName ("TRANSPORT");
  // cannon at daylight
  g_Load3ds.Import3DS (&model_cannon1, dirs->getModels ("cannon1.3ds"));
  // cannon at night
  g_Load3ds.Import3DS (&model_cannon2, dirs->getModels ("cannon2.3ds"));
  model_cannon2.nolight = true;
  model_cannon2.object [0]->vertex [2].color.c [0] = 50;
  model_cannon2.object [0]->vertex [2].color.c [1] = 50;
  model_cannon2.object [0]->vertex [2].color.c [2] = 50;
  model_cannon2.object [0]->vertex [1].color.c [0] = 50;
  model_cannon2.object [0]->vertex [1].color.c [1] = 50;
  model_cannon2.object [0]->vertex [1].color.c [2] = 50;
  g_Load3ds.Import3DS (&model_missile1, dirs->getModels ("missile1.3ds"));
  model_missile1.setName ("AAM MK1");
  g_Load3ds.Import3DS (&model_missile2, dirs->getModels ("missile2.3ds"));
  model_missile2.setName ("AAM MK2");
  g_Load3ds.Import3DS (&model_missile3, dirs->getModels ("missile3.3ds"));
  model_missile3.setName ("AAM MK3");
  g_Load3ds.Import3DS (&model_missile4, dirs->getModels ("missile4.3ds"));
  model_missile4.setName ("AGM MK1");
  g_Load3ds.Import3DS (&model_missile5, dirs->getModels ("missile5.3ds"));
  model_missile5.setName ("AGM MK2");
  g_Load3ds.Import3DS (&model_missile6, dirs->getModels ("missile6.3ds"));
  model_missile6.setName ("DFM");
  g_Load3ds.Import3DS (&model_flak1, dirs->getModels ("flak2.3ds"));
  model_flak1.setName ("SA CANNON");
  g_Load3ds.Import3DS (&model_flarak1, dirs->getModels ("flarak1.3ds"));
  model_flarak1.setName ("SAM");
  g_Load3ds.Import3DS (&model_ship1, dirs->getModels ("ship1.3ds"));
  model_ship1.setName ("CRUISER");
  g_Load3ds.Import3DS (&model_tent1, dirs->getModels ("tent1.3ds"));
  model_tent1.setName ("TENT");
  g_Load3ds.Import3DS (&model_gl117, dirs->getModels ("gl-117.3ds"));
  g_Load3ds.Import3DS (&model_tank1, dirs->getModels ("tank1.3ds"));
  model_tank1.setName ("WIESEL");
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
  g_Load3ds.Import3DS (&model_tent4, dirs->getModels ("tent4.3ds"));
  model_tent4.setName ("BIG TENT");
  g_Load3ds.Import3DS (&model_hall1, dirs->getModels ("hall1.3ds"));
  model_hall1.setName ("HALL");
  g_Load3ds.Import3DS (&model_hall2, dirs->getModels ("hall2.3ds"));
  model_hall2.setName ("HALL");
  g_Load3ds.Import3DS (&model_oilrig, dirs->getModels ("oilrig.3ds"));
  model_oilrig.setName ("OILRIG");
  g_Load3ds.Import3DS (&model_egg, dirs->getModels ("egg.3ds"));
  model_egg.setName ("COMPLEX");
  g_Load3ds.Import3DS (&model_radar, dirs->getModels ("radar.3ds"));
  model_radar.setName ("RADAR");
  g_Load3ds.Import3DS (&model_mine1, dirs->getModels ("mine1.3ds"));
  model_radar.setName ("MINE");
  g_Load3ds.Import3DS (&model_aster1, dirs->getModels ("aster1.3ds"));
  model_radar.setName ("ASTEROID");
  g_Load3ds.Import3DS (&model_base1, dirs->getModels ("base1.3ds"));
  model_radar.setName ("MOON BASE");
/*  for(i = 0; i < model.numOfMaterials; i++)
  {
    if(strlen(model.pMaterials[i]->strFile) > 0)
    {
//      CreateTexture(g_Texture, model.pMaterials[i].strFile, i);
    }
    model.pMaterials[i]->texureId = i;
  }*/

  if (debug) { printf ("\ndone importing models"); fflush (stdout); }

  /* Z-Buffer f. Berechnung verdeckter Flaechen einschalten */
  glEnable(GL_DEPTH_TEST);

  /* Vorder- u. Rueckseite der Polygone nur als Randlinien darstellen */
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

  if (debug) { printf ("\n initing space"); fflush (stdout); }
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

  init_reshape ();

  CColor mycolor (200, 200, 0);
  CColor mycolor2 (100, 100, 0);
  mysphere = new CSphere (1, 8, 1, 1, 1);
  mysphere->setColor (&mycolor2);
  mysphere->setNorthPoleColor (&mycolor, 1.9);
  for (i = 0; i < 100; i++)
  {
    mykugel [i] = new InitKugel (mysphere);
  }

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
//  rot2.a = 270;
  initexplode = 0;
  initexplode1 = 0;

  sungamma = 60;
  setLightSource (60);

  glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  glEnable (GL_LIGHT0);
  glEnable (GL_LIGHTING);
  glEnable (GL_COLOR_MATERIAL);

  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
//  glDisable (GL_DITHER);
}

void init_key (int key, int x, int y)
{
//  game_quit ();
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

void init_display ()
{
  int i;
//  char buf [20];
//  char buf2 [20];
  CVector3 vec;
  CColor color (200, 200, 200, 255);

//  gluPerspective (80.0, (float) width / height, nearclippingplane, 50.0); // should be sqrt(2) or 1.5
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity ();
//  glShadeModel(GL_SMOOTH);
//  glPushMatrix ();
//  gluPerspective (80.0, (float) width / height, nearclippingplane, 150.0); // should be sqrt(2) or 1.5

/*  glBegin (GL_TRIANGLES);
  glColor3ub (255,255,0);
  glVertex3f (-1,-1,-1.1);
  glVertex3f (1,-1,-1.1);
  glVertex3f (0,1,-1.1);
  glEnd ();*/

  glPushMatrix ();

  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);

  glPushMatrix ();
  glTranslatef (0,0,-5);
  model_fig.draw (&vec, &tl, &rot, 1.0, 1.0, initexplode1);
  glPopMatrix ();

  if (initexplode1 < 0)
  {
    glPushMatrix ();
    glTranslatef (0,0,-5);
    model_gl117.draw2 (&vec, &tl2, &rot2, 1.0, initexplode);
    glPopMatrix ();
  }

  glDisable (GL_LIGHTING);
  glTranslatef (1,1,-50);

  glPushMatrix ();
  gl->rotate (0 ,inittimer, 0);
  gl->rotate (0 ,0 ,inittimer);
  int num = 50;
  if (quality < 2) num = 10;
  for (i = 0; i < num; i++)
  {
    mykugel[i]->draw ();
  }
  glPopMatrix ();
  glPushMatrix ();
  gl->rotate (0 ,0 ,360 - inittimer);
  gl->rotate (0 ,360 - inittimer, 0);
  for (i = 100 - num; i < 100; i++)
  {
    mykugel[i]->draw ();
  }
  glPopMatrix ();
  glPopMatrix ();


/*  glPushMatrix ();
  glTranslatef (0,0,-5);
  glEnable (GL_DEPTH_TEST);
  model_fig.draw2 (&vec, &tl2, &rot2, 1.0, initexplode);
  glPopMatrix ();*/

/*  glPushMatrix ();
  glTranslatef (0,0,inittextz);
  gl->rotate ((int) inittextrot, 0, 0);
  strcpy (buf, "GL-117");
  buf2 [1] = 0;
  for (i = 0; i < 10; i++)
  {
    buf2 [0] = buf [i];
    drawText (i-3, -0.5, 0.05, buf2, &color);
    gl->rotate (-18, 0, 0);
  }*/

  glPopMatrix ();
  //drawText (0, 0, -0.9, "LOADING...", &color);

  font2->drawText (-20, -20, -3, VERSIONSTRING, &color);

//  glPopMatrix ();
//  glutSwapBuffers ();
}

void init_timer ()
{
  if (initexplode > 30) initexplode = -1;
  if (initexplode1 > 30) initexplode1 = -1;

  int r = myrandom (100);
  if (r == 50) r = myrandom (100); // do not optimize this

/*  if (inittextz < -0.5)
  {
    inittextz += 0.06;
  }*/
  if (inittimer > 200)
  {
    if (!mykugel [0]->explosion)
      for (int i = 0; i < 100; i ++)
        mykugel [i]->explosion ++;
  }
  tl.x = 8.0 * pow (1.5, -(5 + tl.z)) - 0.4;
  tl.y = 0.5 * tl.x;
  tl.z += 0.15;
  if (initexplode1 == -1 && tl2.z < 3)
  {
    tl2.z += 0.25;
  }
  if (initexplode1 >= 0 && tl.z > 2) initexplode1 ++;
//  tl2.z += 0.15;
  if (!mykugel [0]->explosion)
    inittimer ++;
  if (mykugel [0]->explosion >= 100)
  {
    init_key (27, 0, 0); // switch to menu
  }
//  initexplode ++;
//  inittextrot += 5;
  
//  rot.a ++;
  if (tl2.z < 3)
  {
    rot2.b -= 20;
    if (rot2.b < 0) rot2.b += 360;
  }
  if (tl.z > -4.0)
  {
    rot.b --;
//    rot2.b --;
  }
//  rot.c ++;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

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
}

static void myMouseFunc (int button, int state, int x, int y)
{
  if (game == GAME_PLAY)
  {
    game_mouse (button, state, x, y);
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
}

static void myReshapeFunc (int width, int height)
{
  ::width = width;
  ::height = height;
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_reshape ();
  else if (game == GAME_MENU || game == GAME_MISSION)
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
//    myInit();
//    game = GAME_PLAY;
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
  else if (game == GAME_FINISH)
  {
    if (camera == 50) camera = 0;
    game_display ();
    finish_display ();
    game_view ();
  }
}

static void myIdleFunc ()
{
#ifdef USE_GLUT
  glutPostRedisplay ();
#endif
}

static void myJoystickAxisFunc (int x, int y, int t, int r)
{
  if (game == GAME_PLAY)
  {
    game_joystickaxis (x, y, t, r);
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

#define TIMER_INTERVAL 33
#define GLUT_TIMER_INTERVAL 28

static void myTimerFunc (int value)
{
//  fplayer->aiAction (l);
  if (game == GAME_PLAY)
    game_timer ();
  else if (game == GAME_INIT)
    init_timer ();
  else if (game == GAME_MENU)
  {
    if (!missionactive)
      game_timer ();
    menu_timer ();
  }
  else if (game == GAME_CREDITS)
  {
    credits_timer ();
  }
  else if (game == GAME_CREATE)
    create_timer ();
  else if (game == GAME_JOIN)
    join_timer ();
  else if (game == GAME_MISSION)
    mission_timer ();
  else if (game == GAME_STATS)
    stats_timer ();
  else if (game == GAME_FAME)
    mission_timer ();
  else if (game == GAME_FIGHTER)
    mission_timer ();
  else if (game == GAME_FINISH)
    finish_timer ();

#ifdef USE_GLUT
  glutTimerFunc (GLUT_TIMER_INTERVAL, myTimerFunc, 0);
#endif
}

static void sdlTimerFunc ()
{
  if (game == GAME_PLAY)
    game_timer ();
  else if (game == GAME_INIT)
    init_timer ();
  else if (game == GAME_MENU)
  {
    if (!missionactive)
      game_timer ();
    menu_timer ();
  }
  else if (game == GAME_CREDITS)
  {
    credits_timer ();
  }
  else if (game == GAME_CREATE)
    create_timer ();
  else if (game == GAME_JOIN)
    join_timer ();
  else if (game == GAME_MISSION)
    mission_timer ();
  else if (game == GAME_STATS)
    stats_timer ();
  else if (game == GAME_FAME)
    mission_timer ();
  else if (game == GAME_FIGHTER)
    mission_timer ();
  else if (game == GAME_FINISH)
    finish_timer ();
}

#ifndef USE_GLUT

Uint32 nexttime = 0;

Uint32 sdlTimeLeft ()
{
  Uint32 now;
  now = SDL_GetTicks ();
  if (nexttime <= now)
  {
    nexttime = now + TIMER_INTERVAL;
    return 0;
  }
  return nexttime - now;
}

void sdlWaitTimer ()
{
  Uint32 now;
  now = SDL_GetTicks ();
  if (abs (nexttime - now) > 2000) nexttime = now;
  while (now < nexttime)
  {
    now = SDL_GetTicks ();
  }
  nexttime = now + TIMER_INTERVAL;
}

int joystickx = 0, joysticky = 0, joystickt = 0, joystickr = 0;
int joystickbutton = -1;

void sdlMainLoop ()
{
  SDL_Event event;
  while (1)
  {
    while (SDL_PollEvent (&event))
    {
      switch (event.type)
      {
        case SDL_MOUSEMOTION:
/*          if (game == GAME_PLAY)
            sdl_mousemotion (event.motion.xrel, event.motion.yrel);
          else*/
          myPassiveMotionFunc (event.motion.x, event.motion.y);
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
          break;
        case SDL_JOYAXISMOTION:
//          if (abs (event.jaxis.value) > 500)
          {
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
//            printf ("\naxis=%d, val=%d", event.jaxis.axis, event.jaxis.value); 
          }
          break;
        case SDL_JOYBUTTONDOWN:
          joystickbutton = event.jbutton.button;
          if (joystickbutton >= 2)
            myJoystickButtonFunc (joystickbutton);
          break;
        case SDL_JOYBUTTONUP:
          joystickbutton = -1;
          break;
        case SDL_JOYHATMOTION:
          myJoystickHatFunc (event.jhat.value);
          break;
        case SDL_ACTIVEEVENT:
          sdlreshape = true;
          sdldisplay = true;
          break;
      }
    }
//    printf ("\nx=%d, y=%d", joystickx, joysticky);
    if (controls >= CONTROLS_JOYSTICK)
    {
      myJoystickAxisFunc (joystickx, joysticky, joystickt, joystickr);
      if (joystickbutton == 0)
        myJoystickButtonFunc (0);
      else
        joystickbutton = -1;
    }
    if (sdldisplay) myDisplayFunc ();
    sdldisplay = false;
    if (sdlreshape) myReshapeFunc (width, height);
    sdlreshape = false;
//    SDL_Delay (sdlTimeLeft ());
    sdlWaitTimer ();
    sdlTimerFunc ();
/* Bernd: Please uncomment the following two lines! */
//    if (game == GAME_PLAY)
//      game_easymouse ();
  }
}
#endif

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

bool configinit = false;

void config_test (int argc, char **argv)
{
  printf ("\nNo configuration file found. Testing..."); fflush (stdout);
  int bppi [4];
//  memset (modes, 0, 4 * sizeof (int));

#ifdef USE_GLUT
  printf ("\nUsing GLUT only"); fflush (stdout);
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
  configinit = true;
#else
  printf ("\nUsing SDL and GLUT"); fflush (stdout);
  if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
  {
    fprintf (stderr, "\nCouldn't initialize SDL: %s\n", SDL_GetError ());
    exit (1);
  }
  configinit = true;
#endif

  int valids = -1;
  int n = 0;
  while (n < 4)
  {
    if (setScreen (resolution [n] [0], resolution [n] [1], resolution [n] [2], resolution [n] [3]))
    {
//      modes [n] = speedTest (); // get fps for the current resulotion
      bppi [n] = bpp; // store bpp setting
      valids = n;
      break;
    }
    else
    {
      bppi [n] = -1;
//      modes [n] = -1;
    }
    n ++;
  }

  if (valids == -1)
  {
    fprintf (stderr, "\nFatal: No working display modes found! Try editing the file conf yourself. You may not be able to play this game.");
    fflush (stderr);
    exit (2);
  }

  quality = 0;
  view = 10;
  width = resolution [valids] [0];
  height = resolution [valids] [1];
  bpp = bppi [valids];
  fullscreen = resolution [valids] [3];

/*  if (modes [0] < 40 && bppi [0] > 0)
  {
    quality = 0;
    view = 20;
    width = resolution [0] [0];
    height = resolution [0] [1];
    bpp = bppi [0];
    fullscreen = resolution [0] [3];
    if (modes [0] < 20) view = 20;
    else view = 30;
  }
  else
  {
    if (modes [1] < 70 && bppi [1] > 0)
    {
      width = resolution [1] [0];
      height = resolution [1] [1];
      bpp = bppi [1];
      fullscreen = resolution [1] [3];
      quality = 3;
      if (modes [1] < 40) view = 20;
      else if (modes [1] < 50) view = 30;
      else if (modes [1] < 60) view = 30;
      else view = 40;
      if (modes [1] < 50) quality = 2;
    }
    else if (modes [2] < 70 && bppi [2] > 0)
    {
      width = resolution [2] [0];
      height = resolution [2] [1];
      bpp = bppi [2];
      fullscreen = resolution [2] [3];
      quality = 3;
      if (modes [2] < 40) view = 20;
      else if (modes [2] < 50) view = 30;
      else if (modes [2] < 60) view = 30;
      else view = 40;
      if (modes [2] < 50) quality = 2;
    }
    else
    {
      width = resolution [3] [0];
      height = resolution [3] [1];
      bpp = bppi [3];
      fullscreen = resolution [3] [3];
      if (modes [3] < 40) quality = 2;
      else if (modes [3] < 80) quality = 3;
      else if (modes [3] < 120) quality = 4;
      else quality = 5;
      view = (int) (modes [3] / 10) * 10 - 10;
      if (view > 100) view = 100;
      else if (view < 30) view = 30;
    }
  }*/

#ifdef USE_GLUT
//  glutDestroyWindow (glutwindow);
#else
//  SDL_Quit ();
#endif
}

int main (int argc, char **argv)
{
  printf ("\nStartup %s, %s ... ", argv [0], VERSIONSTRING); fflush (stdout);

#ifdef _MSC_VER
  printf ("\nWindows detected ");
#endif

  dirs = new Dirs (argv [0]);
  
  if (argc > 1)
    debug = true;
//  dirs->getSaves ("filename");

  if (!load_config ())
  {
    config_test (argc, argv);
    firststart = true;
  }

  save_config ();

/*
  time_t t;
  time (&t);
  srandom ((long) t);*/

/*  int time;
#ifndef USE_GLUT
  time = SDL_GetTicks ();
#else
  time = glutGet (GLUT_ELAPSED_TIME);
#endif

  srand (time);*/

  server = NULL;
  client = NULL;

  pilots = new PilotList (dirs->getSaves ("pilots"));

#ifdef USE_GLUT
  printf ("\nUsing GLUT only"); fflush (stdout);
  if (!configinit)
  {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
    if (!setScreen (width, height, bpp, fullscreen))
    {
      fprintf (stderr, "Fatal: No working display mode %dx%d found", width, height);
      fflush (stderr);
      exit (10);
    }
  }
/*  char gamestr [256];
  sprintf (gamestr, "%dx%d:%d", width, height, bpp);
  glutGameModeString (gamestr);
  if (fullscreen && glutGameModeGet (GLUT_GAME_MODE_POSSIBLE))
  {
//    width = 800; height = 600;
    glutEnterGameMode ();
  }
  else
  {
    glutInitWindowPosition (0, 0);
    glutInitWindowSize (width, height);
    if (glutCreateWindow ("GL-117") == GL_FALSE)
      exit (1);
  }*/

//  glutFullScreen ();
  myFirstInit ();

  sound = new SoundSystem ();

  glutReshapeFunc (myReshapeFunc);
  glutDisplayFunc (myDisplayFunc);
  glutKeyboardFunc (myKeyboardFunc);
  glutSpecialFunc (mySpecialFunc);
  glutPassiveMotionFunc (myPassiveMotionFunc);
  glutMouseFunc (myMouseFunc);
  glutIdleFunc (myIdleFunc);
  glutTimerFunc (20, myTimerFunc, 0);

  gluPerspective (80.0, (float) width / height, nearclippingplane, 50.0); // should be sqrt(2) or 1.5
  if (controls <= 0)
    controls = CONTROLS_MOUSE;
  glutMainLoop();
#else
  printf ("\nUsing SDL and GLUT"); fflush (stdout);
  if (!configinit)
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
    {
      fprintf (stderr, "\nCouldn't initialize SDL: %s\n", SDL_GetError ());
      exit (1);
    }
  atexit (SDL_Quit);
#ifdef HAVE_SDL_NET
  // initialize SDL_net
  if (SDLNet_Init () == -1)
  {
    fprintf (stderr, "\nSDLNet_Init: %s\n", SDLNet_GetError ()); fflush (stderr);
    exit (2);
  }
  printf ("\nUsing SDL_net"); fflush (stdout);
#endif

/*  if (bpp == 0) {
    if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel <= 8 ) {
      bpp = 8;
    } else {
      bpp = 16;
    }
  }

  Uint32 video_flags;
  video_flags = SDL_OPENGL;
  if (fullscreen) video_flags |= SDL_FULLSCREEN;

  int rgb_size[3];
  switch (bpp) {
    case 8:
    rgb_size[0] = 2;
    rgb_size[1] = 3;
    rgb_size[2] = 3;
    break;
    case 15:
    case 16:
    rgb_size[0] = 5;
    rgb_size[1] = 5;
    rgb_size[2] = 5;
    break;
    default:
    rgb_size[0] = 8;
    rgb_size[1] = 8;
    rgb_size[2] = 8;
    break;
  }
  SDL_GL_SetAttribute (SDL_GL_RED_SIZE, rgb_size [0]);
  SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, rgb_size [1]);
  SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, rgb_size [2]);
  SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
printf ("\nEntering mode %dx%d:%d,%d ", width, height, bpp, video_flags); fflush (stdout);
  if (SDL_SetVideoMode (width, height, bpp, video_flags) == NULL)
  {
    if (SDL_VideoModeOK (width, height, bpp, video_flags) != 0)
    {
      fprintf (stderr, "\nCouldn't set GL mode: %s\n", SDL_GetError ());
      SDL_Quit ();
      exit (1);
    }
  }*/

  if (!configinit)
  {
    if (!setScreen (width, height, bpp, fullscreen))
    {
      fprintf (stderr, "Fatal: No working display mode %dx%d found", width, height);
      fflush (stderr);
      exit (10);
    }
  }

  SDL_WM_SetCaption ("GL-117", "GL-117");

  SDL_ShowCursor (0);

  if (debug) { printf ("\nnew SoundSystem"); fflush (stdout); }
  sound = new SoundSystem ();
  if (debug) { printf ("\ndone new SoundSystem"); fflush (stdout); }
  sound->volumesound = volumesound;
  sound->volumemusic = volumemusic;
  sound->setVolume ();
  sound->setVolumeMusic ();

  if (debug) { printf ("\nplay Music"); fflush (stdout); }
  sound->playMusic (MUSIC_STANDBY1);

  if (debug) { printf ("\nfirst init for intro"); fflush (stdout); }
  myFirstInit ();
  myReshapeFunc (width, height);

  if (debug) { printf ("\njoysticks"); fflush (stdout); }
  joystick = SDL_NumJoysticks ();
  if (joystick > 0)
  {
    SDL_JoystickEventState (SDL_ENABLE);
    sdljoystick = SDL_JoystickOpen (0);
    printf ("\nJoystick detected."); fflush (stdout);
  }
  else
  {
    printf ("\nNo joystick found."); fflush (stdout);
    sdljoystick = NULL;
    if (controls >= 2)
      controls = CONTROLS_MOUSE;
  }

//  joystick = 0; // disable joystick manually

  SDL_EnableUNICODE (1);
  SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY,
                       SDL_DEFAULT_REPEAT_INTERVAL);

//  SDL_WM_GrabInput (SDL_GRAB_ON);

  sdlMainLoop ();
#endif
  return 0;
}

#endif

