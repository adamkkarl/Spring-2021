function [densityx] = kNN(x,k,D)
%calculate a probability est at x using the kNN method
    N=length(D);

    densityx = []
    for i=1:length(x)
        dists = []
        for j=1:length(D)
            dists = [dists abs(D(j)-x(i))];
        end
        sortedDists = sort(dists);
        V = 2*sortedDists(k);   %kth fathest point
        densityx = [densityx k/(N*V)]
    end