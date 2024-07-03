#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;
int ts = 54; //tile size
Vector2i offset(48,24);
struct chunk
{ int x,y,col,row,kind,similar,pixcelSize;
chunk(){similar=0; pixcelSize=255;}
} grid[10][10];
void swap(chunk p1,chunk p2)
{
std::swap(p1.col,p2.col);
std::swap(p1.row,p2.row);
grid[p1.row][p1.col]=p1;
grid[p2.row][p2.col]=p2;
}
int main()
{
srand(time(0));
RenderWindow app(VideoMode(740,480), "Menagerie");
app.setFramerateLimit(60);
Texture t1,t2;
t1.loadFromFile("images/background.png");
t2.loadFromFile("images/animals.png");
Sprite background(t1), gems(t2);
for (int i=1;i<=8;i++)
for (int j=1;j<=8;j++)
{
grid[i][j].kind=rand()%3;
grid[i][j].col=j;
grid[i][j].row=i;
grid[i][j].x = j*ts;
grid[i][j].y = i*ts;
}
int x0,y0,x,y; int click=0; Vector2i pos;
bool isswap=false, isMoving=false;
while (app.isOpen())
{
Event e;
while (app.pollEvent(e))
{
if (e.type == Event::Closed)
app.close();
if (e.type == Event::MouseButtonPressed)
if (e.mouseButton.button == Mouse::Left)
{
if (!isswap && !isMoving) click++;
pos = Mouse::getPosition(app)-offset;
}
}
// Finding the similar animal images
for(int i=1;i<=8;i++)
for(int j=1;j<=8;j++)
{
if (grid[i][j].kind==grid[i+1][j].kind)
if (grid[i][j].kind==grid[i-1][j].kind)
for(int n=-1;n<=1;n++) grid[i+n][j].similar++;
if (grid[i][j].kind==grid[i][j+1].kind)
if (grid[i][j].kind==grid[i][j-1].kind)
for(int n=-1;n<=1;n++) grid[i][j+n].similar++;
}
// Handling the mouse cliks
if (click==1)
{
x0=pos.x/ts+1;
y0=pos.y/ts+1;
}
if (click==2)
{
x=pos.x/ts+1;
y=pos.y/ts+1;
if (abs(x-x0)+abs(y-y0)==1)
{swap(grid[y0][x0],grid[y][x]); isswap=1; click=0;}
else click=1;
}
// Controling the animations
isMoving=false;
for (int i=1;i<=8;i++)
for (int j=1;j<=8;j++)
{
chunk &p = grid[i][j];
int dx,dy;
for(int n=0;n<4;n++)
{dx = p.x-p.col*ts;
dy = p.y-p.row*ts;
if (dx) p.x-=dx/abs(dx);
if (dy) p.y-=dy/abs(dy);}
if (dx||dy) isMoving=1;
}
// Deletion of similar animal images
if (!isMoving)
for (int i=1;i<=8;i++)
for (int j=1;j<=8;j++)
if (grid[i][j].similar) if (grid[i][j].pixcelSize>10) {grid[i][j].pixcelSize-
=10; isMoving=true;}
// Updating the scores after each similar finding
int score=0;
for (int i=1;i<=8;i++)
for (int j=1;j<=8;j++)
score+=grid[i][j].similar;
//Second swapeIt if no similar
if (isswap && !isMoving)
{if (!score) swap(grid[y0][x0],grid[y][x]); isswap=0;}
//Updating the grid after swaping and deletion
if (!isMoving)
{
for(int i=8;i>0;i--)
for(int j=1;j<=8;j++)
if (grid[i][j].similar)
for(int n=i;n>0;n--)
if (!grid[n][j].similar) {swap(grid[n][j],grid[i][j]); break;};
for(int j=1;j<=8;j++)
for(int i=8,n=0;i>0;i--)
if (grid[i][j].similar)
{
grid[i][j].kind = rand()%7;
grid[i][j].y = -ts*n++;
grid[i][j].similar=0;
grid[i][j].pixcelSize = 255;
}
}
// Drawing the each part on screen
app.draw(background);
for (int i=1;i<=8;i++)
for (int j=1;j<=8;j++)
{
chunk p = grid[i][j];
gems.setTextureRect( IntRect(p.kind*49,0,49,49));
gems.setColor(Color(255,255,255,p.pixcelSize));
gems.setPosition(p.x,p.y);
gems.move(offset.x-ts,offset.y-ts);
app.draw(gems);
}
app.display();
}
return 0;
}