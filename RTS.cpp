//Dec 4 2023
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
# define M_PI 3.14159265358979323846
#define GridSize 500

bool MouseLeft,MouseRight;
int Team=0,UnitSelect=-1;
float Zoom1=1;
struct Unit;
sf::Vector2i PixelXY,ScreenXY=sf::Vector2i(512,512),MapSize;
sf::Vector2f WorldXY;
sf::RenderWindow window;
sf::Event event; 
sf::Texture MapTexture,FowTexture,MaskTexture;
sf::Sprite MapSprite,FowSprite,MaskSprite;
sf::Image FowImage,MaskImage;
sf::View View1(sf::FloatRect(0, 0, (float)ScreenXY.x,(float)ScreenXY.y));
sf::Color TeamColors[]={sf::Color(66,200,88),sf::Color(133,133,22),
sf::Color(200,55,33),sf::Color(66,22,200)};
sf::Keyboard Keyboard1;
sf::Mouse Mouse1;
	
std::vector<Unit> UnitArray;

//struct hello{
//	void hi(){}

//};

template <typename T>
T DegToRad(T degrees)
{
    return(T)(degrees * (M_PI/180));
}

float clamp(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

static float AngleDifference( float angle1, float angle2 )
{
    float diff = fmod(( angle2 - angle1 + 180.f ),360.f )- 180.f;
    return diff < -180 ? diff + 360 : diff;
}

int smallest(int x, int y, int z){
    return std::min(std::min(x, y), z);
}

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    
    bool load( sf::Vector2i tileSize, unsigned int width, unsigned int height)
    {
       
        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Triangles);
        m_vertices.resize(width * height * 6);

        // populate the vertex array, with two triangles per tile
        for (unsigned int x = 0; x < width; ++x)
            for (unsigned int y = 0; y < height; ++y)
            {
                
                // get a pointer to the triangles' vertices of the current tile
                sf::Vertex* triangles = &m_vertices[(x + y * width) * 6];
                // define the 6 corners of the two triangles
                triangles[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
                triangles[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
                triangles[2].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);
                triangles[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);
                triangles[4].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
                triangles[5].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);

         
          /*    if(CurrentCoordinate==sf::Vector2i(x,y)){
                for(int i =0; i<6; i++){
                    triangles[i].color = sf::Color(0,0,0,0);}
             } else {  for(int i =0; i<6; i++){
                    triangles[i].color = sf::Color(255,255,255);}}
					}*/}
        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();
        
        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;

};

  struct Unit{
	  float UnitRotate=90,UnitTargetRotate=90,Speed=0.4f;
  int UnitTeam,UnitID,TravelState=0;
  sf::Vector2f Travel = sf::Vector2f(0, 0),
	  UnitXY,DistanceXY;
  sf::ConvexShape UnitShape; 
	void Begin(){  
		UnitXY.x = (float)(rand() % MapSize.x);
		UnitXY.y = (float)(rand() % MapSize.y);

	UnitShape.setPointCount(3);
	UnitShape.setPoint(0, sf::Vector2f(-33, 30));
	UnitShape.setPoint(1, sf::Vector2f(0, 0));
	UnitShape.setPoint(2, sf::Vector2f(33, 30));
	
    UnitShape.setOutlineThickness(5);
	
	  UnitShape.setOutlineColor(sf::Color(0,0,0));
	UnitShape.setFillColor(TeamColors[UnitTeam]);

	UnitID = (int)(UnitArray.size() - 1);
	}

	void Update()
	{
		UnitShape.setPosition(UnitXY);UnitShape.setRotation(450-UnitRotate);
	   window.draw(UnitShape); UnitShape.setScale(.75f*Zoom1,.75f*Zoom1);
	   switch(TravelState){
	   case 1:
		 UnitRotate = clamp((AngleDifference(UnitRotate,UnitTargetRotate)
			 +UnitRotate),UnitRotate-Speed,UnitRotate+Speed);
		 UnitRotate = fmod(UnitRotate , 360.f); if(UnitRotate<0) {UnitRotate+=360.f;}
		 if((int)UnitRotate==(int)UnitTargetRotate)
		 {TravelState=2;}
		 break;
	   case 2:	
		   UnitXY.x += cos(DegToRad(UnitRotate))*Speed;
			  UnitXY.y += sin(DegToRad(UnitRotate+180))*Speed;
			  DistanceXY.x -= abs(cos(DegToRad(UnitRotate)) * Speed);
			  DistanceXY.y -= abs(sin(DegToRad(UnitRotate + 180)) * Speed);
	   if(DistanceXY.x+DistanceXY.y<=15){TravelState=0;}break;
	   } }};

void ButtonLive()
{
	if (Mouse1.isButtonPressed(Mouse1.Left)){
		if(!MouseLeft)
		{MouseLeft = true;
		if(UnitSelect!=-1){
		UnitArray[UnitSelect].
			UnitShape.setOutlineColor(sf::Color(0,0,0));  
		UnitSelect= -1;/*std::cout<<"hi\n";*/}

		if(UnitSelect=-1){
	for(Unit i:UnitArray)
	{if(i.UnitTeam==Team)
	{if(i.UnitShape.getGlobalBounds().contains(WorldXY)){
		UnitSelect=i.UnitID;
	UnitArray[UnitSelect].UnitShape.setOutlineColor(sf::Color(255,255,255));
	break;} }}}}

	} else{MouseLeft=false; }

	if (Mouse1.isButtonPressed(Mouse1.Right)){if(UnitSelect>=0){ if(!MouseRight)
		{MouseRight = true;
	UnitArray[UnitSelect].Travel= WorldXY; UnitArray[UnitSelect].TravelState=1;
	sf::Vector2f Vec2F1 = UnitArray[UnitSelect].Travel-UnitArray[UnitSelect].UnitXY;
	UnitArray[UnitSelect].UnitTargetRotate=((int)((180.f-(atan2(Vec2F1.y ,Vec2F1.x)
		) / M_PI) * 180.f )% 360);
	UnitArray[UnitSelect].DistanceXY = WorldXY-UnitArray[UnitSelect].UnitXY;
	UnitArray[UnitSelect].DistanceXY.x = abs(UnitArray[UnitSelect].DistanceXY.x);
	UnitArray[UnitSelect].DistanceXY.y = abs(UnitArray[UnitSelect].DistanceXY.y);
	}}}else{MouseRight=false;}

 if (Keyboard1.isKeyPressed(Keyboard1.W))
 {  View1.move(0,Zoom1*-.1f); }	 
 if (Keyboard1.isKeyPressed(Keyboard1.S))
 { View1.move(0,Zoom1*.1f);  } 
 if (Keyboard1.isKeyPressed(Keyboard1.A))
 { View1.move(Zoom1*-.1f,0);}
 if (Keyboard1.isKeyPressed(Keyboard1.D))
 { View1.move(Zoom1*.1f,0);}

  if (Keyboard1.isKeyPressed(Keyboard1.C))
 { 
	  View1.zoom(.9998f); Zoom1*=.9998f; }
    if (Keyboard1.isKeyPressed(Keyboard1.Z))
 { 
		View1.zoom(1.0002f); Zoom1*=1.0002f;  }
}

int main()
{  
	window.create(sf::VideoMode(ScreenXY.x,ScreenXY.y), "NationOnDust", sf::Style::Close | sf::Style::Resize);
	
	srand((unsigned) time(NULL));

MapTexture.loadFromFile("Assets/WorldMap.png");
	MapSprite.setTexture(MapTexture);
	
	MapSize=(sf::Vector2i)MapTexture.getSize();
	View1.setCenter(MapSize.x/2.f,MapSize.y/2.f);
	
	FowImage.create(MapSize.x,MapSize.y,sf::Color::Green);
	FowTexture.loadFromImage(FowImage);
	FowSprite.setTexture(FowTexture,true);

	MaskImage.create(MapSize.x/2,MapSize.y/2,sf::Color(111,0,0,255));
	MaskTexture.loadFromImage(MaskImage);
	MaskSprite.setTexture(MaskTexture,true);

	for(int i=0;i<6;i++)
	{   Unit Un1;
	UnitArray.push_back (Un1);
	UnitArray[i].UnitTeam=i%4;
	UnitArray[i].Begin();
	}	UnitArray[0].UnitXY.x= MapSize.x/2.f; UnitArray[0].UnitXY.y= MapSize.y/2.f; 

	while (window.isOpen())
	{
	
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
window.close();
			}
			if (event.type == sf::Event::Resized)
	{  ScreenXY.x=event.size.width;	ScreenXY.y=event.size.height;
				View1.setSize((sf::Vector2f)ScreenXY);Zoom1=1;		}
		}
	
	ButtonLive();
	 PixelXY=Mouse1.getPosition(window);
	 WorldXY=window.mapPixelToCoords(PixelXY);
		
	 window.clear(sf::Color(0,0,37));
	 window.setView(View1);


	window.draw(MapSprite); 

	for(int i=0; i< UnitArray.size();i++)
 	{UnitArray[i].Update();}

		//window.draw(FowSprite, sf::RenderStates(sf::BlendMultiply));
		//window.draw(MaskSprite);

	window.display();
	}
	
	return 0;
}
