function [ascores] = AUROC_score(x, y)
    class_0 = [];
    class_1 = [];
    ascores = [];
    
    for i=1:size(x, 2)
        [X,Y,T,AUC] = perfcurve(y, x(:,i), 1);
        
        ascores = [ascores; i, AUC];
    end
    
    %sort descending by AUROC score
    ascores = sortrows(ascores, 2, 'descend');   
end