function varargout = AutoPlugGUI(varargin)
% AUTOPLUGGUI MATLAB code for AutoPlugGUI.fig
%      AUTOPLUGGUI, by itself, creates a new AUTOPLUGGUI or raises the existing
%      singleton*.
%
%      H = AUTOPLUGGUI returns the handle to a new AUTOPLUGGUI or the handle to
%      the existing singleton*.
%
%      AUTOPLUGGUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in AUTOPLUGGUI.M with the given input arguments.
%
%      AUTOPLUGGUI('Property','Value',...) creates a new AUTOPLUGGUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before AutoPlugGUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to AutoPlugGUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help AutoPlugGUI

% Last Modified by GUIDE v2.5 07-Dec-2012 16:03:29

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @AutoPlugGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @AutoPlugGUI_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before AutoPlugGUI is made visible.
function AutoPlugGUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to AutoPlugGUI (see VARARGIN)



  clc
   %% 
   loop=500;%count values 
   count = 2;
   stop_flag = 0;
   stop_flag1 = 0;
   points_count = 1;
   speed_buffer = 0;
   distance_buffer = 0;
   points_to_plot_x = [30 10 15 21 11];
   points_to_plot_y = [10 6 17 21 11];
   t = 0;
   %%
   
   %% Plot Details of distance 
   distance_handle = plot(handles.distance_axes,t,distance_buffer,'r');
   set(handles.distance_axes,'Color',[0.15 0.15 0.15]);
   xlim(handles.distance_axes,[0 loop]);
   ylim(handles.distance_axes,[0 160]);
   grid(handles.distance_axes,'on');
%    xlabel(handles.distance_axes,'Time','FontSize',12,'Color',[0.502 0.502 0.502]);   
   ylabel(handles.distance_axes,'Distance(m)','FontSize',12,'Color',[0.502 0.502 0.502]);
   set(handles.distance_axes, 'XColor', [0.502 0.502 0.502],'YColor',[0.502 0.502 0.502]);  
   %%
   
   %% Plot Details of Speed 
   speed_handle = plot(handles.speed_axes,t,speed_buffer,'b');
   set(handles.speed_axes,'Color',[0.15 0.15 0.15]);
%     line([0 loop], [40 40],'markersize',7,'color',[1 0 0]); % line 1 - Inner track - lower horizontal 
%    line([0 loop], [40 40],'LineWidth',2);
   xlim(handles.speed_axes,[0 loop]);
   ylim(handles.speed_axes,[0 160]);
   grid(handles.speed_axes,'on');
%    xlabel(handles.speed_axes,'Time','FontSize',12,'Color',[0.502 0.502 0.502]);
   ylabel(handles.speed_axes,'Speed(kmph)','FontSize',12,'Color',[0.502 0.502 0.502]);
    set(handles.speed_axes, 'XColor', [0.502 0.502 0.502],'YColor',[0.502 0.502 0.502]);  
   axis off;
   %% 
   
   %% Plot Details of the track
%    track_handle = plot(handles.track_axes,track_buffer,'r');

   track_radius = 60;
   track_radius2 = 200;

   theta1 = pi/2:0.01:pi;
   theta2 = pi:0.01:(3*pi)/2;
   theta3 = 0:0.01:pi/2;
   theta4 = -pi/2:0.001:0;
    
   track_center_1 = [200 200];
   track_center_2 = [200 170];
   track_center_3 = [540 200];
   track_center_4 = [540 170];
     
   track1_xunit1 = track_radius * cos(theta1) + track_center_1(1);
   track1_yunit1 = track_radius * sin(theta1) + track_center_1(2);
   
   track1_xunit2 = track_radius2 * cos(theta1) + track_center_1(1);
   track1_yunit2 = track_radius2 * sin(theta1) + track_center_1(2);
   
   track2_xunit1 = track_radius * cos(theta2) + track_center_2(1);
   track2_yunit1 = track_radius * sin(theta2) + track_center_2(2);
   
   track2_xunit2 = track_radius2 * cos(theta2) + track_center_2(1);
   track2_yunit2 = track_radius2 * sin(theta2) + track_center_2(2);
   
   track3_xunit1 = track_radius * cos(theta3) + track_center_3(1);
   track3_yunit1 = track_radius * sin(theta3) + track_center_3(2);
   
   track3_xunit2 = track_radius2 * cos(theta3) + track_center_3(1);
   track3_yunit2 = track_radius2 * sin(theta3) + track_center_3(2);
   
   track4_xunit1 = track_radius * cos(theta4) + track_center_4(1);
   track4_yunit1 = track_radius * sin(theta4) + track_center_4(2);
   
   track4_xunit2 = track_radius2 * cos(theta4) + track_center_4(1);
   track4_yunit2 = track_radius2 * sin(theta4) + track_center_4(2);


   track_handle = plot(handles.track_axes,track1_xunit1,track1_yunit1,track1_xunit2,track1_yunit2,'markersize',17,'color',[0 0.498 0]);
   hold on;
   plot(handles.track_axes,track2_xunit1,track2_yunit1,track2_xunit2,track2_yunit2,'markersize',17,'color',[0 0.498 0]);
   hold on;
   plot(handles.track_axes,track3_xunit1,track3_yunit1,track3_xunit2,track3_yunit2,'markersize',17,'color',[0 0.498 0]);
   hold on;
   plot(handles.track_axes,track4_xunit1,track4_yunit1,track4_xunit2,track4_yunit2,'markersize',17,'color',[0 0.498 0]);
   hold on;

   
   line([200 540], [110 110],'markersize',7,'color',[0 0.498 0]); % line 1 - Inner track - lower horizontal 
   line([200 540], [260 260],'markersize',7,'color',[0 0.498 0]); % line 2 - Inner track - upper horizontal
   line([100 640], [0 0],'markersize',7,'color',[0 0.498 0]);     % line 3 - Outer track - lower horizontal 
   line([100 640], [380 380],'markersize',7,'color',[0 0.498 0]); % line 4 - Outer track - upper horizontal
   
   line([140 140],[150 220],'markersize',7,'color',[0 0.498 0]);  % line 5 - Inner track - left vertical 
   line([0 0],[140 230],'markersize',7,'color',[0 0.498 0]);      % line 6 - Outer track - left vertical
   
   line([600 600],[150 220],'markersize',7,'color',[0 0.498 0]);  % line 7 - Inner track - right vertical 
   line([740 740],[150 240],'markersize',7,'color',[0 0.498 0]);  % line 8 - Outer track - right vertical 
    
   xlim(handles.track_axes,[0 740]);
   ylim(handles.track_axes,[0 380]);
   set(handles.track_axes,'Color',[0 0 0]);
   set(handles.track_axes,'xtick',[],'ytick',[]);
   %%

   %% Values for radar plot

%    radar_theta1 = 87.5;
% 
%    
%    radar_line1_x = [0 150]/tand(radar_theta1);
%    radar_line1_y = [0 150]/sind(radar_theta1);
%    
%    radar_line2_x = [0 150]/tand(-radar_theta1);
%    radar_line2_y = [0 150]/sind(radar_theta1);
%    plot(handles.radar_axes,radar_line1_x,radar_line1_y,radar_line2_x,radar_line2_y,'markersize',7,'color',[1 0 0]);
% %    hold on;
%    set(handles.radar_axes,'Color',[0.3 0.3 0.3]);
%    title(handles.radar_axes,'Radar Vision');
%    xlim(handles.radar_axes,[-150 150]);
%    ylim(handles.radar_axes,[0 150]);
   
   
   %% 
  
   %% Initial plot details
   
   
   %%
   
   %%  Serial Port Configuration
  
   delete(instrfindall) ;

   s = serial('COM4'); %assigns the object s to serial port
 
   set(s, 'InputBufferSize',100) % number of bytes in inout buffer

   set(s, 'BaudRate',115200);

   fopen(s);  %opens the serial port

   %%
   
      
   %% Storing the handles for subsequent use in other callback function
   
   handles.count  = count;
   handles.loop   = loop;
   handles.distance_handle = distance_handle;
   handles.speed_handle = speed_handle;
%    handles.radar_handle = radar_handle;
   handles.track_handle = track_handle;
   handles.points_to_plot_x = points_to_plot_x;
   handles.points_to_plot_y = points_to_plot_y;
   handles.Distance = 0;
   handles.Angle = 0;
   handles.SetSpeed = 0;
   handles.FrontCarSpeed = 0;
   handles.points_count = points_count;
   handles.s = s;
   handles.stop_flag = 0;
   handles.anand = 10;
%    set (handles.stop_flag1,'String','0');
   %%
   
   % Initial values of safe distance , radar angle and set speed
   % safe_distance = 25
   % radar_angle = 5;
   % set_speed = 0;
    plot_radar(87.5,handles,0,0);
   array = [170,204,0,2,55,59,25,6,0];
   sendpacket(array,handles);
  % Choose default command line output for AutoPlugGUI
   handles.output = hObject;
    
  % Update handles structure
   guidata(hObject, handles);

   
% UIWAIT makes AutoPlugGUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = AutoPlugGUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


    
% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in startButton.
function Start_Callback(hObject, eventdata, handles)
% hObject    handle to startButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

   
   
   clc;
   %% Retrieving the handle values 
%    count = handles.count;
%    loop  = handles.loop;
   speed_handle = handles.speed_handle;
   distance_handle = handles.distance_handle;
%    track_handle = handles.track_handle;
   points_count = handles.points_count;
   points_to_plot_x = handles.points_to_plot_x;
   points_to_plot_y = handles.points_to_plot_y;
   
   
%    disp 'Started'
    handles.stop_flag = 0; %handles.stop_flag;
   %% 
   GraphingTimeDelay = 0.1; % The length of time that Matlab should pause between positions when graphing, if at all, in seconds.
   s = handles.s;
   pause on;
   

%     plot(handles.track_axes,10,10);

   speed_max_deviation = 70;
   dist_max_deviation = 100;
   t = 0;
   value = str2double(get(handles.frontCarSpeed, 'String'));
   lead_car_speed = value;
   speed_buf = 0 ;
   distance_buf = 0;
   count = 2;
   loop = 500;
  while ((count ~= loop) && handles.stop_flag == 0)
%    while ~isequal(count,loop)
     
%    if handles.stop_flag == 1
%        disp 'Voila'
%    end

%    disp 'Voila'
   input_params = fread(s);

   if(input_params(1) == 170 && input_params(2) == 204)

    speed = typecast(uint8(input_params(3)),'int8');
    speed = speed*(18/5);
     
%     if ((count>=4 && speed == 0) || (count>=4 && (speed - speed_buf(count-1)) > speed_max_deviation ))
%          speed = speed_buf(count-1); 
%     end 

    distance_h = input_params(15);
    distance = (256 * distance_h + input_params(16));
     
    car1_x_pos_h = input_params(19);
    car1_x_pos =  car1_x_pos_h * 4;
    
    car1_y_pos_h = input_params(20);
    car1_y_pos = car1_y_pos_h * 2;
    
     car2_x_pos_h = input_params(21);
     car2_x_pos = car2_x_pos_h * 4;
    
     car2_y_pos_h = input_params(22);
     car2_y_pos = car2_y_pos_h * 2;


     front_car_speed = str2double(get(handles.frontCarSpeed, 'String'));
     value = str2double(get(handles.radar_angle, 'String'));
   
     if distance == 2500
          plot_radar(90 - value/2,handles,0,0);         
    else
        plot_radar(90 - value/2,handles,0,distance);     
     end
    
%      if distance == 2500
%         distance = 150;
%      end
    
%     if ( distance ~= 2500 && (count>=4 && (distance - distance_buf(count-1)) > dist_max_deviation ))
%          distance = distance_buf(count-1);
%     end
    
    
    
    if (count>=4 && distance == 2500) 
       distance_buf = [distance_buf distance_buf(count-1)]; 
    elseif(count <4 && distance == 2500)
         distance_buf = [distance_buf 0];  
    else
        distance_buf = [distance_buf distance];
    end    

%     distance_buf = [distance_buf distance];
    speed_buf = [speed_buf speed]; 
    t = [t count];
    lead_car_speed =[lead_car_speed front_car_speed];

%     set(distance_handle,'xdata',t);
%     set(distance_handle,'ydata',distance_buf);
    
    if distance == 2500
        axes(handles.distance_axes);
        dist_handle1 = plot(count,distance_buf(count-1),'-.*r','markersize',2,'color',[1 1 0]);
%         distance_buf = [distance_buf distance_buf(count-1)];
%         dist2 = [dist2 2500];
%         dist_handle1 = plot(t,dist2,'markersize',7,'color',[0 1 0]);
%         dist_handle2 = plot(t,distance_buf,'markersize',7,'color',[0 0 1]);
    else
%         distance_buf = [distance_buf distance];
%         dist2 = [dist2 0];
        axes(handles.distance_axes);
%         dist_handle3 = plot(t,dist2,'markersize',7,'color',[0 1 0]);
        dist_handle4 = plot(t,distance_buf,'markersize',7,'color',[1 0 0]);
    end   
    
   xlim(handles.distance_axes,[0 loop]);
   ylim(handles.distance_axes,[0 160]);
   grid(handles.distance_axes,'on');
%    xlabel(handles.distance_axes,'Time','FontSize',16);
   ylabel(handles.distance_axes,'Distance(m)','FontSize',12,'Color',[0.502 0.502 0.502]);
   
   set(handles.distance_axes,'Color',[0.15 0.15 0.15]);
    set(handles.distance_axes, 'XColor', [0.502 0.502 0.502],'YColor',[0.502 0.502 0.502]);  
   
    axes(handles.speed_axes);
    temp_handle = plot(t,speed_buf,'markersize',7,'color',[1 0 0]);
    hold on;
    temp_handle1 = plot(t,lead_car_speed,'markersize',7,'color',[0 1 0]);
    hold on;
    set(handles.speed_axes,'Color',[0.15 0.15 0.15]);
    xlim(handles.speed_axes,[0 loop]);
   ylim(handles.speed_axes,[0 160]);
   grid(handles.speed_axes,'on');
%    xlabel(handles.speed_axes,'Time','FontSize',16);
   ylabel(handles.speed_axes,'Speed (kmph)','FontSize',12,'Color',[0.502 0.502 0.502]);
    set(handles.speed_axes, 'XColor', [0.502 0.502 0.502],'YColor',[0.502 0.502 0.502]);  
%     set(speed_handle,'xdata',t);
%     set(speed_handle,'ydata',speed_buf);
%     axes(handles.speed_axes);
%     line([0 150],[count count]);
%     points_count = mod(points_count , 5);
   
    count = count + 1;
    hold(handles.distance_axes , 'on');
    hold(handles.speed_axes , 'on');
    
%    track_handle1 = plot(handles.track_axes,car1_x_pos,car1_y_pos,'MarkerFaceColor','r','markersize',7);
%    track_handle2 = plot(handles.track_axes,car2_x_pos,car2_y_pos,'MarkerFaceColor','b','markersize',7);
   
      track_handle1 = plot(handles.track_axes,car1_x_pos,car1_y_pos,'-.or','markersize',7,'MarkerFaceColor',[1 0 0]);
   track_handle2 = plot(handles.track_axes,car2_x_pos,car2_y_pos,'-.or','markersize',7,'MarkerFaceColor',[0 1 1],'MarkerEdgeColor',[0 1 1]);
%    track_handle1 = plot(handles.track_axes,car1_x_pos,car1_y_pos,'-.or','markersize',7,'color',[1 0 0]);
%    track_handle2 = plot(handles.track_axes,car2_x_pos,car2_y_pos,'-.or','markersize',7,'color',[0 0 1]);
   
   pause(0.001);
   set(track_handle1,'Visible','off');
   set(track_handle2,'Visible','off');
   set(handles.track_axes,'Color',[0 0 0]);
     handles = guidata(hObject);
   end
  
  end

% --- Executes on button press in stopButton.
function Stop_Callback(hObject, eventdata, handles)
% hObject    handle to stopButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
disp 'Stopped'
% stop_flag = handles.stop_flag; 
% % stop_flag
% stop_flag = 1;
% stop_flag

%  setappdata(handles.anand,'String','2');

handles.stop_flag = 1;
guidata(hObject, handles);
% fclose(handles.s);
% delete(handles.s);
% clear handles.s;
% stop_flag = 1;





function safe_dist_Callback(hObject, eventdata, handles)
% hObject    handle to safe_dist (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of safe_dist as text
%        str2double(get(hObject,'String')) returns contents of safe_dist as a double


% --- Executes during object creation, after setting all properties.
function safe_dist_CreateFcn(hObject, eventdata, handles)
% hObject    handle to safe_dist (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function radar_angle_Callback(hObject, eventdata, handles)
% hObject    handle to radar_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of radar_angle as text
%        str2double(get(hObject,'String')) returns contents of radar_angle as a double


% --- Executes during object creation, after setting all properties.
function radar_angle_CreateFcn(hObject, eventdata, handles)
% hObject    handle to radar_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function sendpacket(array,handles)
chksum = 0;
for i = 1:size(array,2)
    tmp = array(i);
   chksum = bitxor(chksum,tmp);
end
array(size(array,2) + 1) = chksum;
% array
 fwrite(handles.s,array,'uchar');


function plot_radar(angle,handles,x,y)
  
   GraphingTimeDelay = 0.1;
    cla(handles.radar_axes);
    axes(handles.radar_axes);
   
   radar_line1_x = [20 150]/tand(angle);
   radar_line1_y = [20 150]/sind(angle);
   
   radar_line2_x = [20 150]/tand(-angle);
   radar_line2_y = [20 150]/sind(angle);
   
   radar_handle = plot(radar_line1_x,radar_line1_y,radar_line2_x,radar_line2_y,'markersize',7,'color',[0 0.498 0]);
   hold on;
   radar_handle2 = plot(0,10,'-.or','markersize',10,'MarkerFaceColor',[0.847 0.161 0]);
   hold on;
   set(handles.radar_axes,'Color',[0 0 0]);
   if(y~= 0)
       radar_handle1 = plot(x,y,'-.or','markersize',7,'MarkerFaceColor',[0 1 1],'MarkerEdgeColor',[0 1 1]);
        xlim(handles.radar_axes,[-150 150]);
       ylim(handles.radar_axes,[0 150]);
       pause(GraphingTimeDelay);
   end
   
   value = str2double(get(handles.radar_angle, 'String'));  
   value = ((value/2) * pi)/180;
   theta = pi/2 - value :0.01:pi/2 + value;
   center = [0 0];
   hektor = (10 * pi)/ 180;
%    theta_scan = pi/2 - hektor :0.01:pi/2 + hektor;
   
   radius = 150;

  for i = 20: 25: 150
      x = i * cos(theta) + center(1);
      y = i * sin(theta) + center(2);
%       scan_x = radius * cos(theta_scan) + center(1);
%       scan_y = radius * sin(theta_scan) + center(2);
      plot(handles.radar_axes,x,y,'markersize',17,'color',[1 1 0]);
%       hold on;
%       line(handles.radar_axes,[0 0],[0 120]);
%       hold on;
%       plot(handles.radar_axes,scan_x,scan_y,'markersize',17,'color',[1 1 0]);
      pause(0.0001);
      xlim(handles.radar_axes,[-150 150]);
      ylim(handles.radar_axes,[0 150]); 
%       set(blah1,'Visible','off');
  end
      set(handles.radar_axes,'xtick',[],'ytick',[]);
  
  
 

% --- Executes on button press in radarMinus.
function radarMinus_Callback(hObject, eventdata, handles)

%% Retrieving the current value from the text box
%% and updating with the decremented value 
cla(handles.radar_axes);
value = str2double(get(handles.radar_angle, 'String'));
value = value - 1;
  if(value <= 5)
    value = 5;
  end  
set(handles.radar_angle, 'String', value);
%%

handles.Angle = value;
% if handles.Distance <25
%     handles.Distance = 25;
% end
% 
% if handles.setSpeed <50
%     handles.setSpeed = 50;
% end
% 
% if handles.FrontCarSpeed <59
%     handles.FrontCarSpeed = 59;
% end

plot_radar(90 - value/2,handles,0,0);
  
% Forming the array to be send to the MATLAB Gateway
array = [170,204,0,2,55,0,0,handles.Angle,0];
sendpacket(array,handles);
guidata(hObject, handles);



% --- Executes on button press in radarPlus.
function radarPlus_Callback(hObject, eventdata, handles)

cla(handles.radar_axes);
%% Retrieving the current value from the text box
%% and updating with the incremented value 
%  clf(handles.radar_axes)   
value = str2double(get(handles.radar_angle, 'String'));
value = value + 1;
   if(value >= 40)
     value = 40;
   end 
   set(handles.radar_angle, 'String', value);
%%

handles.Angle = value;
% if handles.Distance <25
%     handles.Distance = 25;
% end
% 
% if handles.FrontCarSpeed <59
%     handles.FrontCarSpeed = 59;
% end

plot_radar(90 - value/2,handles,0,0);

% Forming the array to be send to the MATLAB Gateway
array = [170,204,0,2,55,0,0,handles.Angle,0];
sendpacket(array,handles);
guidata(hObject, handles);



% --- Executes on button press in dist_minus.
function dist_minus_Callback(hObject, eventdata, handles)

value = str2double(get(handles.safe_dist, 'String'));
value = value - 1;
  if(value <= 25)
     value = 25;
  end
set(handles.safe_dist, 'String', value);

handles.Distance = value;
% if handles.Angle <5
%     handles.Angle = 5;
% end
% if handles.SetSpeed <50 && handles.SetSpeed ~= 0
%     handles.SetSpeed = 50;
% end
% 
% if handles.FrontCarSpeed <59
%     handles.FrontCarSpeed = 59;
% end

% Forming the array to be send to the MATLAB Gateway
array = [170,204,0,2,55,0,handles.Distance,0,0];
sendpacket(array,handles);
guidata(hObject, handles);


% --- Executes on button press in dist_Plus.
function dist_Plus_Callback(hObject, eventdata, handles)
value = str2double(get(handles.safe_dist, 'String'));
value = value + 1;
  if(value >= 70)
     value = 70;
  end
set(handles.safe_dist, 'String', value);

handles.Distance = value;

% if handles.Angle <5
%     handles.Angle = 5;
% end
% 
% if handles.SetSpeed <50 && handles.SetSpeed ~= 0
%     handles.SetSpeed = 50;
% end
% 
% if handles.FrontCarSpeed <59
%     handles.FrontCarSpeed = 59;
% end

% Forming the array to be send to the MATLAB Gateway
array = [170,204,0,2,55,0,handles.Distance,0,0];
sendpacket(array,handles);
guidata(hObject, handles);



% --- Executes on button press in setSpeedMinus.
function setSpeedMinus_Callback(hObject, eventdata, handles)
% hObject    handle to setSpeedMinus (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
value = str2double(get(handles.setSpeed, 'String'));
value = value - 1;
  if(value <= 50)
    value = 50;
  end  
set(handles.setSpeed, 'String', value);
%%

handles.SetSpeed = value;

% if handles.Angle <5
%     handles.Angle = 5;
% end
% 
% if handles.Distance <25
%     handles.Distance = 25;
% end
% 
% if handles.FrontCarSpeed <59
%     handles.FrontCarSpeed = 59;
% end

array = [170,204,0,2,55,0,0,0,handles.SetSpeed];
sendpacket(array,handles);
guidata(hObject, handles);


% --- Executes on button press in setSpeedPlus.
function setSpeedPlus_Callback(hObject, eventdata, handles)
% hObject    handle to setSpeedPlus (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
value = str2double(get(handles.setSpeed, 'String'));
value = value + 1;
  if(value >= 150)
    value = 150;
  end  
set(handles.setSpeed, 'String', value);

handles.SetSpeed = value;
% 
% if handles.Angle <5
%     handles.Angle = 5;
% end
% 
% if handles.Distance <25
%     handles.Distance = 25;
% end
% 
% if handles.FrontCarSpeed <59
%     handles.FrontCarSpeed = 59;
% end

array = [170,204,0,2,55,0,0,0,handles.SetSpeed];
sendpacket(array,handles);
guidata(hObject, handles);

% --- Executes on button press in frontCarSpeedMinus.
function frontCarSpeedMinus_Callback(hObject, eventdata, handles)

value = str2double(get(handles.frontCarSpeed, 'String'));
value = value - 1;
  if(value <= 50)
    value = 50;
  end  

set(handles.frontCarSpeed, 'String', value);

handles.FrontCarSpeed = value;

% if handles.Angle <5
%     handles.Angle = 5;
% end
% 
% if handles.Distance <25
%     handles.Distance = 25;
% end
% 
% if handles.SetSpeed <50 && handles.SetSpeed ~=0
%     handles.SetSpeed = 50;
% end

array = [170,204,0,2,55,handles.FrontCarSpeed,0,0,0];
sendpacket(array,handles);
guidata(hObject, handles);

% --- Executes on button press in frontCarSpeedPlus.
function frontCarSpeedPlus_Callback(hObject, eventdata, handles)

value = str2double(get(handles.frontCarSpeed, 'String'));
value = value + 1;
  if(value >= 120)
    value = 120;
  end  
set(handles.frontCarSpeed, 'String', value);

handles.FrontCarSpeed = value;

% if handles.Angle <5
%     handles.Angle = 5;
% end
% 
% if handles.Distance <25
%     handles.Distance = 25;
% end
% 
% if handles.SetSpeed <50 && handles.SetSpeed ~=0
%     handles.SetSpeed = 50;
% end

array = [170,204,0,2,55,handles.FrontCarSpeed,0,0,0];
sendpacket(array,handles);
guidata(hObject, handles);


function setSpeed_Callback(hObject, eventdata, handles)
% hObject    handle to setSpeed (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of setSpeed as text
%        str2double(get(hObject,'String')) returns contents of setSpeed as a double


% --- Executes during object creation, after setting all properties.
function setSpeed_CreateFcn(hObject, eventdata, handles)
% hObject    handle to setSpeed (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function frontCarSpeed_Callback(hObject, eventdata, handles)
% hObject    handle to frontCarSpeed (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of frontCarSpeed as text
%        str2double(get(hObject,'String')) returns contents of frontCarSpeed as a double


% --- Executes during object creation, after setting all properties.
function frontCarSpeed_CreateFcn(hObject, eventdata, handles)
% hObject    handle to frontCarSpeed (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in LeftLane.
function LeftLane_Callback(hObject, eventdata, handles)

array = [170,204,1,2,55,0,0,0,0];
sendpacket(array,handles);
guidata(hObject, handles);


% --- Executes on button press in RightLane.
function RightLane_Callback(hObject, eventdata, handles)

array = [170,204,2,2,55,0,0,0,0];
sendpacket(array,handles);
guidata(hObject, handles);



% Old Code

%   car1_x_pos_h = input_params(19);
%   car1_x_pos = (256 * car1_x_pos_h + input_params(20));
%     
%   car1_y_pos_h = input_params(21);
%   car1_y_pos = (256 * car1_y_pos_h + input_params(22));
%     
%    car2_x_pos_h = input_params(23);
%    car2_x_pos = (256 * car2_x_pos_h + input_params(24));
 
%    angle
%    radar_line1 = [0 150]/sind(angle);
%    radar_line2 = [0 150]/sind(angle);
   
%    temp = [0 150]/sind(90-angle);
%    set(handles.radar_handle,'xdata',[0 -150]);
%     set(handles.radar_handle,'ydata',temp);
%     hold on;
%     drawnow;
%    radar_handle = plot(handles.radar_axes,[0 150],radar_line1,[0 -150],radar_line2);
%    hold on;
%    set(radar_handle,'Xdata',20,'Ydata',20);
%    plot(handles.radar_axes,20,20,'-.or','markersize',7,'color',[1 0 0]); 
%    title(handles.radar_axes,'Radar - Field of View');
%    xlim(handles.radar_axes,[-50 50]);
%    ylim(handles.radar_axes,[0 150]);

  %    track_handle = plot(handles.track_axes,points_to_plot_x(points_count),points_to_plot_y(points_count),'-.or','markersize',7);
%    track_handle = plot(handles.track_axes,10,20,'-.or','markersize',7);
%     plot(handles.track_axes,50,70,'-.or','markersize',7);
%    track_handle = plot(handles.track_axes,points_to_plot_x(points_count),points_to_plot_y(points_count),'-.or','markersize',7); 
   
%    track_radius = 10;
%    track_radius2 = 20;
   
%    track1_center_1 = [45 25];
%    track1_center_2 = [110 25];
%    track2_center_1 = [25 25];
%    track2_center_2 = [130 25];
   
   
%    theta1 = pi/2:0.01:(3*pi)/2;
%    theta2 = -pi/2:0.001:pi/2;
   

%    track1_center_1 = [45 25];
%    track1_center_2 = [110 25];
%    track2_center_1 = [25 25];
%    track2_center_2 = [130 25];


    % Old
%    track1_xunit1 = track_radius * cos(theta1) + track1_center_1(1);
%    track1_yunit1 = track_radius * sin(theta1) + track1_center_1(2);
%    
%    track1_xunit2 = track_radius * cos(theta2) + track1_center_2(1);
%    track1_yunit2 = track_radius * sin(theta2) + track1_center_2(2);
%    
%    track2_xunit1 = track_radius2 * cos(theta1) + track2_center_1(1);
%    track2_yunit1 = track_radius2 * sin(theta1) + track2_center_1(2);
%    
%    track2_xunit2 = track_radius2 * cos(theta2) + track2_center_2(1);
%    track2_yunit2 = track_radius2 * sin(theta2) + track2_center_2(2);
     
%    track_handle = plot(handles.track_axes,track1_xunit1,track1_yunit1,track1_xunit2,track1_yunit2);
%      plot(handles.track_axes,track2_xunit1,track2_yunit1,track2_xunit2,track2_yunit2);







