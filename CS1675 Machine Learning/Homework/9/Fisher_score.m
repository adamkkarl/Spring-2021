function [fscores] = Fisher_score(x, y)
    class_0 = [];
    class_1 = [];
    fscores = [];
    
    for i=1:size(x, 1)
        if y(i,1) == 0
            class_0 = [class_0; x(i,:)];
        else
            class_1 = [class_1; x(i,:)];
        end
    end
    
    for i=1:size(x, 2)
        c0_mean = mean(class_0(:,i));
        c0_std = std(class_0(:,i));
        c1_mean = mean(class_1(:,i));
        c1_std = std(class_1(:,i));
        
        fisher = (c0_mean - c1_mean)^2 / (c0_std^2 + c1_std^2);
        
        fscores = [fscores; i, fisher];
    end
    
    %sort descending by fisher score
    fscores = sortrows(fscores, 2, 'descend');   
end