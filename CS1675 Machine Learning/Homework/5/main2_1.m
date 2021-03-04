function main2_1()

    pima = readmatrix("pima.txt");
    
    class_1 = [];
    class_0 = [];
    
    for i=1:size(pima, 1)
        if pima(i,9) == 1
            class_1 = [class_1; pima(i,:)];
        else
            class_0 = [class_0; pima(i,:)];
        end
    end
    
    histogram_analysis(class_1(:,1));
end