clc
clear all
close all

dir = 1;
max = 8;

F = figure;
hold on
axis([1, max, 1, max, 1 max]);
set(gca, 'XTickLabel', 0:0:0);
set(gca, 'YTickLabel', 0:0:0);
set(gca, 'ZTickLabel', 0:0:0);
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on

s = zeros(max,max,max);
Y = 0;
YY = 0;
X = 0;
XX = 0;
Z = 0;
ZZ = 0;
phase1 = 0;
phase2 = 0;
wipePos = 1;
wipeDir = 1;
spinAngle = 0;
growCubeSize = 1;
growCubeDir = 1;

for x = 1:1:max
   for y = 1:1:max
      s(x,y,1) = 1; 
   end
end

for i = 1:1:1000
%     %SPIRAL
%     for z = 1:1:max;
%             X = sin(phase1 + (z - 1)/(max - 1)*(2*pi - 0) + 0);
%             Y = cos(phase1 + (z - 1)/(max - 1)*(2*pi - 0) + 0);
%             XX = (X - -1)/(1 - -1)*(max - 1) + 1;
%             YY = (Y - -1)/(1 - -1)*(max - 1) + 1;
%             s(round(XX),round(YY),z) = 1;
%             
%             X = sin(pi + phase1 + (z - 1)/(max - 1)*(2*pi - 0) + 0);
%             Y = cos(pi + phase1 + (z - 1)/(max - 1)*(2*pi - 0) + 0);
%             XX = (X - -1)/(1 - -1)*(max - 1) + 1;
%             YY = (Y - -1)/(1 - -1)*(max - 1) + 1;
%             s(round(XX),round(YY),z) = 1;
%     end
    
% %     WAVES
%     for x = 1:1:max
%        for z = 1:1:max
%            X = sin(phase1+(x - 1)/(max - 1)*(2*pi - 0) + 0);
%            Z = cos(phase2+(z - 1)/(max - 1)*(2*pi - 0) + 0);
%            Y = X + Z;
%            YY = round((Y - -2)/(2 - -2)*(max - 1) + 1);
%            s(x,YY,z) = 1;
%        end
%     end

    %SIDE WIPE
    for x = 1:1:max
        for y = 1:1:max
           for z = 1:1:max
              if z == wipePos
                 s(x,y,z) = 1; 
              end
           end
        end
    end

%  %   SPIN
%     for x = 1:1:max
%         for y = 1:1:max
%             for z = 1:1:max
%                 X = (((x-1)/(max-1)*(1 - -1)+ -1)*sin(spinAngle)- -1)/(1 - -1)*(max - 1) + 1;
%                 Y = (((y-1)/(max-1)*(1- -1)+ -1)*cos(spinAngle)- -1)/(1 - -1)*(max - 1) + 1;
%                if abs((X-Y)/(X+Y)) < 0.06
%                     s(x,y,z) = 1;
%                end
%             end
%         end
%     end

%     %GROW CUBE
%     for x = 1:1:max
%        for y = 1:1:max
%            for z = 1:1:max
%               if ((ceil(abs(y-(max+1)/2)) == growCubeSize && ceil(abs(x-(max+1)/2))<= growCubeSize && ceil(abs(z-(max+1)/2))<= growCubeSize))
%                  s(x,y,z) = 1; 
%               end
%               if (ceil(abs(z-(max+1)/2)) == growCubeSize && ceil(abs(y-(max+1)/2))<= growCubeSize && ceil(abs(x-(max+1)/2))<= growCubeSize)
%                   s(x,y,z) = 1;
%               end
%               if (ceil(abs(x-(max+1)/2)) == growCubeSize && ceil(abs(y-(max+1)/2))<= growCubeSize && ceil(abs(z-(max+1)/2))<= growCubeSize)
%                   s(x,y,z) = 1;
%               end
%            end
%        end
%     end
     
    for x = 1:1:max
        for y = 1:1:max
            for z = 1:1:max
                if s(x,y,z) == 1
                    plot3(x,y,z,'bo','MarkerFaceColor','b', 'MarkerSize',7);
                end
            end
        end
    end
    
    s = zeros(max,max,max);
    wipePos = wipePos + wipeDir;
    if wipePos > max
        wipePos = max-1;
        wipeDir = wipeDir*-1;
    elseif wipePos < 1
        wipePos = 2;
        wipeDir = wipeDir*-1;
    end
    
    growCubeSize = growCubeSize + growCubeDir;
    if growCubeSize > max/2
        growCubeSize = (max/2)-1;
        growCubeDir = growCubeDir*-1;
    elseif growCubeSize < 1
        growCubeSize = 2;
        growCubeDir = growCubeDir*-1;
    end
    
    spinAngle = spinAngle + pi/16;
    phase1 = phase1 + pi/7;
    phase2 = phase2 + pi/9;
    pause(0.1);
    cla(F)
end

hold off
close all
