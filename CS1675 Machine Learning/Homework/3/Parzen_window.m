function [densityx] = Parzen_window(x,h,D)
%calculate a probability est given x using a Parzen window
    for i=1:N
        temp = D(i,:);
        temp = abs(temp - x);
        
%         fprintf('Debug:: temp=');
%         disp(temp);
        if temp <= v
            disp('temp < v');
            K = K+1;
        end
    end
end

