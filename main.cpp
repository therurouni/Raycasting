#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;




int main() {

  // Block1 => Window          + 3 circles (green +red)

  RenderWindow window(VideoMode({800, 800}), "Working");
  vector<vector<int>> tilemap(20, vector<int>(20));
  int tilesize = 40;

  // red circle
  CircleShape redcircle(20);
  redcircle.setPosition(Vector2f(400.f,400.f));
  redcircle.setFillColor(Color(255, 0, 0));

  // Blue circle
  CircleShape bluecircle(20);
  bluecircle.setPosition(Vector2f(400.f, 400.f));
  bluecircle.setFillColor(Color(0, 0, 255));

  bool drag = false,drag2=false;
  Vector2f dragOffset;
  int activeCircle = -1; // 0-> red 1-> blue

  // run the program as long as the window is open
  while (window.isOpen()) {
    window.clear(sf::Color::Black);

    // check all the window's events that were triggered since the last
    // iteration of the loop
    while (
        const std::optional event = window.pollEvent()) { // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();

      if (const auto *msp = event->getIf<Event::MouseButtonPressed>()) {
        if (msp->button == Mouse::Button::Left) {

            //-----Red Circle-----------(Check if mouse is inside the circle by calc if mouse pos:  x^2 + y^2 <= r^2)
          Vector2f rtl = redcircle.getPosition();
          float rr = redcircle.getRadius();
          Vector2f rc = rtl + Vector2f(rr, rr);
          Vector2f mousepos = Vector2f(msp->position.x, msp->position.y);

          float mspx = mousepos.x, mspy = mousepos.y;
          float dx = mspx - rc.x, dy = mspy - rc.y;
          float dis = (dx * dx) + (dy * dy);
          if (dis <= rr*rr) {
            activeCircle = 0;
            drag = true;
            dragOffset = mousepos - rc;
          }

          //-----Blue Circle-----------
          Vector2f btl = bluecircle.getPosition();
          float br = bluecircle.getRadius();
          Vector2f bc = btl + Vector2f(br, br);

          dx = mspx - bc.x, dy = mspy - bc.y;
          dis = (dx * dx) + (dy * dy);
          if (dis <= br*br) {
            activeCircle = 1;
            drag = true;
            dragOffset = mousepos - bc;
          }
        }
        else if(msp->button==Mouse::Button::Right){
            drag2 =true;
        }
      }

      //-----Handle dragging of circle to change pos------------------
      if(const auto *msmv = event->getIf<Event::MouseMoved>()){
          if(drag){
              if(activeCircle==0){
                  Vector2f mp(msmv->position.x,msmv->position.y);
                  float rad = redcircle.getRadius();
                  Vector2f newCenter = mp - dragOffset;
                  Vector2f newTopLeft = newCenter - Vector2f(rad,rad);
                  redcircle.setPosition(newTopLeft);
              }
              if(activeCircle==1){
                    Vector2f mp(msmv->position.x,msmv->position.y);
                    float rad = bluecircle.getRadius();
                    Vector2f newCenter = mp - dragOffset;
                    Vector2f newTopLeft = newCenter - Vector2f(rad,rad);
                    bluecircle.setPosition(newTopLeft);
              }
          }
          if(drag2){
              float mspx = msmv->position.x,mspy = msmv->position.y;
              int r = floor(mspx/tilesize),c=floor(mspy/tilesize);
              tilemap[r][c]=1;
          }
      }

      //--------Handling release or drag stop event---------------
      if(const auto *msr = event->getIf<Event::MouseButtonReleased>()){
          drag=false;
          drag2=false;
      }
    }

    // drawing the grid
    for (int row = 0; row < 20; row++) {
      for (int col = 0; col < 20; col++) {
        RectangleShape cell({(float)tilesize, (float)tilesize});
        cell.setPosition(Vector2f(row * tilesize, col* tilesize));

        cell.setOutlineThickness(1);
        if (tilemap[row][col] == 1) {
          cell.setFillColor(Color(0, 0, 255));
        } else {
          cell.setFillColor(Color(30, 30, 30));
        }

        cell.setOutlineColor(Color(80, 80, 80));
        window.draw(cell);
      }
    }
    Vector2f rtl = redcircle.getPosition();
    float rr = redcircle.getRadius();
    Vector2f rc = rtl + Vector2f(rr, rr);
    Vector2f btl = bluecircle.getPosition();
    float br = bluecircle.getRadius();
    Vector2f bc = btl + Vector2f(br, br);

    for(float t=0;t<=1;t+=0.05){
        float x = rc.x + (bc.x - rc.x) * t;
        float y = rc.y + (bc.y - rc.y) * t;
        CircleShape point(2);
        int r = x/tilesize,c=y/tilesize;
        if(tilemap[r][c]==1){
            CircleShape circle(6);
            circle.setFillColor(Color::White);
            circle.setPosition(Vector2f(x,y));
            window.draw(circle);
            break;
        }
        point.setFillColor(Color::White);
        point.setPosition(Vector2f(x,y));
        window.draw(point);
    }
    window.draw(redcircle);
    window.draw(bluecircle);

    // window.draw(...);

    // end the current frame
    window.display();
  }
}
