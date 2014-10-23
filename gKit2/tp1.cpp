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
    float angle=45;
    float znear=100.0;
    float zfar=500.0;
    //montrer triangle, sphere ou sphere perturbe

    //string text = argv[1];
    if (argc<=1)
    {
        cout<<"Vous devez choisir un forme a afficher\n";
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
        }else if(argc == 9){
            r = strtof(argv[2], NULL);
            cx = strtof(argv[3], NULL);
            cy = strtof(argv[4], NULL);
            cz = strtof(argv[5], NULL); 
            angle = strtof(argv[6], NULL); 
            znear = strtof(argv[7], NULL); 
            zfar = strtof(argv[8], NULL); 
            cout<<"Radius "<<r<<"\n";
            cout<<"centre sphere (repere propre)"<<cx<<","<<cy<<","<<cz<<"\n";
            cout<<"Angle znear far "<<angle<<" "<<znear<<" "<<zfar<<"\n";
            cout<<"+-------------------------------------------------------------------+\n";
        }else if(argc == 2){
            r  = 0.5f;
            cx = 0.0f;
            cy = 0.0f;
            cz = -256.0f;
        }
    }else if(forme == 3) {//sphere reyes
        if(argc == 6){
            r = strtof(argv[2], NULL);
            cx = strtof(argv[3], NULL);
            cy = strtof(argv[4], NULL);
            cz = strtof(argv[5], NULL); 
        }else if(argc == 2){
            r  = 50.0f;
            cx = 0.0f;
            cy = 0.0f;
            cz = -256.0f;
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
        //pour la version lancer de rayon, je peut utiliser gk::basicray, j'ai un origin et une direction
        // avec ce rayon je trouve la distance du poin d'intersection avec l'sphere.
        //http://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection

        //Je connais l'origin et le fin de mon rayon dans le repere viewport, mais ma sphere est au moins
        //dans le repere view-camera j'asume que la matrice view camera et model sont l'identite.
        //Apres, perspective et viewport je vais les faire. Je dois multiplier les points fin de mes
        //rayons et comme ca j'aurai l'interesection des rayons avec le sphere. Apres je dois remetre
        //ces intersections de retour au repere de l'image en multipliant par la matrice inverse.

        gk::Transform perspective = gk::Perspective (angle*M_PI/180.0, 1, znear, zfar);
        gk::Transform viewport = gk::Viewport ( width,  height);
        //gk::Transform translate = gk::Translate ( gk::Point(cx,cy,cz));
        gk::Transform T = viewport*perspective;//*translate;
        gk::Transform TInv = T.inverse();

        cout << "sphere\n";
        cout << "T\n";

        T.matrix().print();
        cout << "Tinv\n";
        T.matrix().print();
        cout << "\n";

        //float racine=0,norme=0; //constante dans ce cas
        //float dtotal=0;
        float zdiff=zfar-znear;
        cout<<r<<" "<<cx<<" "<<cy<<" "<<cz<<" \n";
        for(int y= 0; y < width; y++) {
            for(int x= 0; x < height; x++){
                //chaque pixel dans mon image est l'origine d'un rayon je dois transformer le rayon
                //l'origin est a 0 le point final est a 1 (repere image) je transforme au repere view camera
                float fx=(float)(x);
                float fy=(float)(y);
                gk::Point origRay = gk::Point(fx,fy,0.0);
                gk::Point finRay = gk::Point(fx,fy,1.0);

                origRay=TInv(origRay);
                finRay=TInv(finRay);

                //je dois trouver la direction du rayon...
                gk::BasicRay rayon= gk::BasicRay(origRay,finRay);
                gk::Vector l= rayon.d;
                l/=l.Length();

                //le centre sphere est dans le repere viewcamera deja
                gk::Point cSphere = gk::Point(cx,cy,cz);


                //dans ce point l'origin et le fin du rayon sont dans le repere view camera
                //je dois trouver l'intersection du rayon avec le sphere et apres retourner a l'image pour dessiner..
                gk::Vector OmC = origRay-cSphere;
                float normeOmC=OmC.Length();
                float loc=l.x*OmC.x + l.y*OmC.y + l.z*OmC.z;

                float racine= loc*loc - normeOmC*normeOmC + r*r;

                if(x%128==0 && y%128==0){

                    cout<<"x, y "<<x<<","<<y<<"\n";
                    cout<<"Origin Rayon "<<origRay.x<<","<<origRay.y<<","<<origRay.z<<"\n";
                    cout<<"Fin Rayon "<<finRay.x<<","<<finRay.y<<","<<finRay.z<<"\n";
                    cout<<"Direction Rayon "<<l.x<<","<<l.y<<","<<l.z<<"\n";
                    cout<<"OmC "<<OmC.x<<","<<OmC.y<<","<<OmC.z<<"\n";
                    cout<<"+-------------------------------------------------------------------+\n";

                }

                if(racine<0){
                    image->setPixel(x, y, gk::VecColor(0,0,0)); 
                }else{
                    //transformer le point d'interesection ....
                    //je viens de me rendre compte que je n'ai pas besoin de transformer le point XD
                    float dist = sqrt(racine)-loc; 
                    image->setPixel(x, y, gk::VecColor(dist/r,0.0,0.0));    // colorie chaque pixel en rouge
                }  

                //je profite de savoir que la direction du rayon est toujours Z
                //norme o - c
                /*float dx=(x-cx),dy=(y-cy),dz=(z-cz);
                norme=dx*dx + dy*dy + dz*dz;
                //on ne prends pas en compte dx et dy por la racine parce qu'ils sont multiplie par (0 0 1)
                //en plus la norme a une racine carre mais dans la racine principal elle a un puissance 2 alors...
                //on ne la met pas
                racine=dz*dz-norme+r*r;

                if(racine<0){
                    image->setPixel(x, y, gk::VecColor(0,0,0)); 
                }else{
                    image->setPixel(x, y, gk::VecColor(sqrt(racine)/r,sqrt(racine)/r,sqrt(racine)/r));    // colorie chaque pixel en rouge
                }     */     
                image->setPixel(10, 10, gk::VecColor(1,1,1));           
            }
        }  
    }
    else if(forme==3.0f){
        cout << "sphere reyes\n";
        float racine=0,norme=0; //constante dans ce cas
        //float dtotal=0;

        cout<<r<<" "<<cx<<" "<<cy<<" "<<cz<<" \n";


        for(int y= 0; y < width; y++) {
            for(int x= 0; x < height; x++){
                float dx=(x-cx),dy=(y-cy),dz=(z-cz);
                norme=dx*dx + dy*dy + dz*dz;
                //jouer..
                float zp=sqrt(r*r-x*x-y*y);

                racine=dz*dz-norme+(r+zp)*(r+zp);
                //cout<<norme<<" "<<racine<<" \n";

                if(racine<0){
                    image->setPixel(x, y, gk::VecColor(0,0,0)); 
                }else{
                    float dp;
                    //float dn;
                    dp=-1*dz+sqrt(racine);
                    //dp=-1*dz-sqrt(racine);  
                    //dtotal=max(dp,dn);
                    image->setPixel(x, y, gk::VecColor(dp/r,0, 0));    // colorie chaque pixel en rouge
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

