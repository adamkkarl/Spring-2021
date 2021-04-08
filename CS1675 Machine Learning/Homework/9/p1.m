function [bestidx, bestC, bestScore, bestClusters, allClusters] = p1
    clusteringdata = readmatrix("clustering_data.txt");
    bestScore = 0;
    bestidx = [];
    bestC = [];
    bestClusters = [];
    allClusters = [];

    for i=1:30 %30 times
       [idx, C]= kmeans(clusteringdata, 4);
       score = 0;
       
       for e = 1:size(clusteringdata, 1)
           pair = [clusteringdata(e,:); C(idx(e,:), :)];
           dist = pdist(pair);
           score = score + dist^2;
       end
       
       clusters = [sum(idx(:)==1), sum(idx(:)==2), sum(idx(:)==3), sum(idx(:)==4)];
       allClusters = [allClusters; clusters];
       
       if i==1 || score < bestScore
           bestScore = score;
           bestidx = idx;
           bestC = C;
           bestClusters = clusters;
       end
    end
    
end