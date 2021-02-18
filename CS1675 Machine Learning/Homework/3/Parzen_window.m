function [densityx] = Parzen_window(x,h,D)
%calculate a probability est given x using a Parzen window
    N=length(D);
    V=h;         %since centered on x

    [densityx] = []
    for i=1:length(x)
        K=0;
        for j=1:length(D)
            if (x(i)-(h/2)) < D(j)
                if D(j) < (x(i)+(h/2))
                    K=K+1;   %increment number in region
                end
            end
        end
        densityx = [densityx K/(N*V)]
    end