%% ��MATLAB�У��ֱ���Ժڰ�ͼ���Ҷ�ͼ�����ɫͼ��������ɫͼ��ʵ��ͼ��Ķ��롢��ʾ������ȹ��ܡ�
figure(1);
Abw=imread('circles.png');  %�ڰ�ͼ
imshow(Abw);
imwrite(Abw,'circles_1.png');
figure(2);
Bgr=imread('cameraman.tif');    %�Ҷ�ͼ
imshow(Bgr);
imwrite(Bgr,'cameraman_1.tif');
figure(3);
Crgb=imread('glaux3.bmp');  %���ɫͼ
imshow(Crgb);
imwrite(Crgb,'glaux3_1.bmp');
figure(4);
[Dind,map]=imread('index.bmp'); %������ɫͼ
imshow(Dind);
figure(5);
imshow(Dind,map);
imwrite(Dind,map,'index_1.bmp');
fft


%% RGBͨ��
A=imread('youhua.png');
sizex=size(A,2);
sizey=size(A,1);
r=A;
g=A;
b=A;
c=A;
m=A;
y=A;
r(:,:,[2 3])=0;
figure(1);
imshow(r);
g(:,:,[1 3])=0;
figure(2);
imshow(g);
b(:,:,[1 2])=0;
figure(3);
imshow(b);
c(:,:,1)=0;
figure(4);
imshow(c);
m(:,:,2)=0;
figure(5);
imshow(m);
y(:,:,3)=0;
figure(6);
imshow(y);


%% �Զ�������ͼ

ind=zeros(512,'uint8');

% ���ĵ�
x0=256;
y0=256;

% ��ʼ��
x1=256;
y1=128;

% ת��
a1=pi/6;
a2=-pi/6;

% ���뾶��
r=128;

for angle=-pi/2:pi/3:7*pi/6
    x1=x0+cos(angle)*r;
    y1=y0+sin(angle)*r;
    
    [k,b]=CreateLine(x0,y0,x1,y1);
    
    for i=1:512
        for j=1:512
            if ((Judge(j,i,x0+tan(pi/6)*r*cos(angle-a1),y0+tan(pi/6)*r*sin(angle-a1),x1,y1))*(Judge(j,i,x0+tan(pi/6)*r*cos(angle-a1),y0+tan(pi/6)*r*sin(angle-a1),x0,y0))*(Judge(j,i,x0+tan(pi/6)*r*cos(angle+a1),y0+tan(pi/6)*r*sin(angle+a1),x1,y1))*(Judge(j,i,x0+tan(pi/6)*r*cos(angle+a1),y0+tan(pi/6)*r*sin(angle+a1),x0,y0))>0)&&((sqrt((j-x0)^2+(i-y0)^2)<r))&&((sqrt((j-x1)^2+(i-y1)^2)<r))
                if((angle==pi/2||angle==-pi/2)&&(abs(j-x0)<=sqrt(3)*r/6))
                    
                    ind(i,j)=36*(angle/pi)+100;
                    
                elseif(((abs(k*j-i+b)/sqrt(k^2+1))<=sqrt(3)*r/6))
                    ind(i,j)=36*(angle/pi)+100;
                end
            end
        end
    end
end

map1=rand(256,3);
figure(2);
subplot(1,2,1);
imshow(ind);

colorbar();
subplot(1,2,2);
imshow(ind,map1);
colorbar();



%% G=aF+b
a=input("Please input a:");
b=input("Please input b:");
F=imread('cameraman.tif');
G=a*F+b;
imshow(G);







