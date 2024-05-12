clear
clc

time_length = 10;
line_color = ['r', 'g', 'b', 'c', 'm', 'y', 'k', 'w'];

close all

s = serialport('COM12', 115200);
configureTerminator(s,'CR/LF')

readline(s); % 짤린 data 버림
string_data = readline(s);
data = split(split(string_data,", "),":")
data_label = data(:,1);

data_num = size(data_label,1);

h = [];
ax = gca;
ylabel_line = [];

figure("name","plotter")
for k = 1:data_num
    h = [h , animatedline('Color',line_color(k),'Marker','.')];
    ylabel_line = [ylabel_line; data_label(k)];
end
xlabel('Time(sec)');
datetick('x','keeplimits');
ylabel(ylabel_line);
ylim("tickaligned")

startTime = datetime('now');
while 1
    string_data = readline(s);
    data = split(split(string_data,','),':');
    data_val = str2double(data(:,2));

    t =  datetime('now') - startTime;
    for k = 1:data_num
        val = data_val(k)
        addpoints(h(k),datenum(t),val);
        gca.XLim = [datenum(t-seconds(time_length)) datenum(t)];
    end
    drawnow 
end