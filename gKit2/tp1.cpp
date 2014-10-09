#include <unistd.h>
#include <stdio.h>
#include <Triangle.h>
#include "Geometry.h"      // type vecteur, point, couleur, etc. de "base"
#include "Image.h"    // classe image
#include "ImageIO.h"  // entrees / sorties sur des images
#include <math.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	float x0,y0,x1,y1,x2,y2,z0,z1,z2,z;
    float r,cx,cy,cz;
	z0=0.0f;z1=0.0f;z2=0.0f;z=0;
	int width=512,height=512;
    //montrer triangle, sphere ou sphere perturbe

    //string text = argv[1];
    if (argc<=1)
    {
        cout<<"Vous devez choisir un forme a afficher";
        return 0;
    }
    
    float forme=strtof(argv[1], NULL);

    if(forme == 1) {//triangle
        if(argc == 8){
            x0 = strtof(argv[2], NULL);
            y0 = strtof(argv[3], NULL);
            x1 = strtof(argv[4], NULL);
            y1 = strtof(argv[5], NULL);
            x2 = strtof(argv[6], NULL);
            y2 = strtof(argv[7], NULL);     
        }else if(argc == 2){
            x0 = 236.0f;
            y0 = 236.0f;
            x1 = 276.0f;
            y1 = 236.0f;
            x2 = 256.0f;
            y2 = 276.0f;
        }
    }else if(forme == 2) {//sphere
        if(argc == 6){
            r = strtof(argv[2], NULL);
            cx = strtof(argv[3], NULL);
            cy = strtof(argv[4], NULL);
            cz = strtof(argv[5], NULL); 
        }else if(argc == 2){
            r  = 200.0f;
            cx = 256.0f;
            cy = 256.0f;
            cz = 256.0f;
        }
    }else{
        cout<<"Vous devez choisir un forme a afficher";
        return 0;
    }
	

    gk::Point p0 = gk::Point(x0,y0,z0);
    gk::Point p1 = gk::Point(x1,y1,z1);
    gk::Point p2 = gk::Point(x2,y2,z2);

    float minx=x0,miny=y0,maxx=x0,maxy=y0;
    minx=fmin(fmin(x2,x1),x0);
    miny=fmin(fmin(y2,y1),y0);
    maxx=fmax(fmax(x2,x1),x0);
    maxy=fmax(fmax(y2,y1),y0);

    if(maxy>height){maxy=height;}
    if(maxx>width){maxx=width;}
    if(minx<0){minx=0;}
    if(minx<0){minx=0;}

    cout<<maxx<<" "<<maxy<<" "<<minx<<" "<<miny<<" \n";

    //gk::BBox bb=gk::BBox(gk::Point(minx,miny,z),gk::Point(maxx,maxy,z));

    gk::Triangle t0=gk::Triangle(p0,p1,p2);
    gk::Point p;

    gk::Image *image= gk::createImage(width,height);    // cree une image de 512x512 pixels

    //image->setPixel(0, 0, gk::VecColor(1, 1, 1));
    // parcourir tous les pixels de l'image
    if(forme==1.0f){
        float a0=t0.area(),a1,a2,a3;
        for(int y= 0; y < maxy; y++) {
            for(int x= 0; x < maxx; x++){
                p=gk::Point(x,y,z);
                a1=gk::Triangle(p0,p1,p).area();
                a2=gk::Triangle(p1,p2,p).area();
                a3=gk::Triangle(p2,p0,p).area();
                if(a1+a2+a3==a0){
            		//point dans trinagle, dessinez.
               		image->setPixel(x, y, gk::VecColor(1,0, 0));    // colorie chaque pixel en rouge
            	}
            	
            }    // chaque colonne
        	
        }   // chaque ligne
    }else if(forme==2.0f){
        cout << "sphere\n";
        float racine=0,norme=0; //constante dans ce cas
        float dtotal=0;

        cout<<r<<" "<<cx<<" "<<cy<<" "<<cz<<" \n";
        for(int y= 0; y < width; y++) {
            for(int x= 0; x < height; x++){
                //je profite de savoir que la direction du rayon est toujours Z
                //norme o - c
                float dx=(x-cx),dy=(y-cy),dz=(z-cz);
                norme=dx*dx + dy*dy + dz*dz;
                /*on ne prends pas en compte dx et dy por la racine parce qu'ils sont multiplie par (0 0 1)
                en plus la norme a une racine carre mais dans la racine principal elle a un puissance 2 alors...
                on ne la met pas*/
                racine=dz*dz-norme+r*r;
                //cout<<norme<<" "<<racine<<" \n";

                if(racine<0){
                    image->setPixel(x, y, gk::VecColor(0,0,0)); 
                }else{
                    float dp,dn;
                    dp=-1*dz+sqrt(racine);
                    dp=-1*dz-sqrt(racine);  
                    dtotal=max(dp,dn);
                    image->setPixel(x, y, gk::VecColor(dtotal/r,0, 0));    // colorie chaque pixel en rouge
                }                    
            }   
        }  
    }

    //image->setPixel(20,20, gk::VecColor(1,1,1));
    //image->setPixel(40,40, gk::VecColor(1,1,1));
    //image->setPixel(60,60, gk::VecColor(1,1,1));

    // enregistre le resultat
    gk::ImageIO::writeImage("out.bmp", image);

    delete image;
    return 0;
}
