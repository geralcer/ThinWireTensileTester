function [YM,UTS,Break] =  tensilePlotEng(DATA)
% This function plots the Stress strain curves, It takes a table of data as
% an input, with the first colum is load data, second is position data and
% third is motor step counter

% Extract the relevant data from the tables
testStart = 10;
testEnd = height(DATA(:,1))-10;

% Define the specific test varriables in mm and M respectively
gaugeLength = 25;
diameter = 0.04;
CA = diameter^2*pi/4;

% This varriable selects wether position data is selected from the sensor(2)
% or from the motor revolutions(3) 
positionDATA = 2;
x = DATA(testStart:testEnd,positionDATA);
y = DATA(testStart:testEnd,1);
X = table2array(x);
Y = table2array(y);

% Smooth out the displacement data, as ADC was inconsistent
xfilter = smoothdata(X);
xfilter = xfilter .* 1.015 + 17.4;

% Now convert load and displacement to Stress and strain
engY = Y/CA;
engX = (xfilter-gaugeLength)./gaugeLength;

if positionDATA == 2
    engX = X./gaugeLength;
end


% Now correct for a zero offset, sample length being slightly larger or
% smaller than measured.
if engX(10) < 0
    engX = engX -engX(10);
else
    engX = engX + engX(10);
end

% Now correct for the compliance in the machine
compliance = Y .* 0.00027 ;  %Experimental value from different copper sizes
engX = engX - compliance;


% Plot the significant data
plot(engX,engY)
hold on


% Prompts the user for the lower stress input to obtain YM
prompt = 'Enter the value for low stress: ';
lowStress = input(prompt);
lowDifference = abs(engY-lowStress);
[~,closeLowIndex] = min(lowDifference);

% Prompt the user for the high stress input to obtain YM
prompt = 'Enter the best estimate for the end of the elastic region: ';
highStress = input(prompt);
difference = abs(engY - highStress);
[~,closeHighIndex] = min(difference);

% calculates teh YM using a line of best fin in the bounds
B = polyfit(engX(closeLowIndex:closeHighIndex),engY(closeLowIndex:closeHighIndex),1);
x = [engX(1) engX(end)];
y = x.*B(1) + B(2);
YM = B(1);

% Now obtain the ultimate tensile strenth and youngs modulous.
[UTS,loc] = max(engY);
Break = engX(loc);

% uEStress = engY(closeHighIndex);
% uEStrain = engX(closeHighIndex);
% lEStress = engY(closeLowIndex);
% lEStrain = engX(closeLowIndex);
% 
% xElastic = [lEStrain, uEStrain];
% yElastic = [lEStress, uEStress];

% Now plot the best estimate for the youngs modulous line
plot(x,y,"k--")

%Format the Labels 
xlabel("Strain [%]","FontSize",20)
ylabel("Stress [Pa]","FontSize",20)

ylim([0 (max(engY)*1.2)]);
xlim([engX(1) engX(end)])




