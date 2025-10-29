function [] = loadPlot(DATA)
    % This function plots the load displacement curve from the test data

    testStart = 10;
    testEnd = height(DATA(:,1)) - 10;

    x = DATA(testStart:testEnd, 2);
    y = DATA(testStart:testEnd, 1);

    X = table2array(x);
    Y = table2array(y);

    % Smooth data
    xfilter = smoothdata(X);
    yfilter = smoothdata(Y);

    % Set starting point to zero
    xStart = xfilter(1);
    yStart = yfilter(1);

    % Shift data to start from zero
    xfilter = xfilter - xStart;
    yfilter = yfilter - yStart;

    % Now plot the main data
    figure;
    plot(xfilter, yfilter, 'b-', 'LineWidth', 1.5);
    xlabel("X displacement [mm]");
    ylabel("Load [N]");
    title("Load Displacement Curve");
    grid on;
    hold off;
end
