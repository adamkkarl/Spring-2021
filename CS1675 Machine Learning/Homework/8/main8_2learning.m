function main8_2learning
    pneumonia = readmatrix("pneumonia.tex", 'FileType','text');
    
    true_pne = [];
    false_pne = [];
    
    for i=1:size(pneumonia,1)
        if pneumonia(i,5) > 0
            true_pne = [true_pne; pneumonia(i,1:4)];
        else
            false_pne = [false_pne; pneumonia(i,1:4)];
        end
    end
    
    disp("=======  general params  ========");
    params = sum(pneumonia, 1)/size(pneumonia, 1);
    disp(params);
    
    disp("======= pneumonia = true ========");
    params = sum(true_pne, 1)/size(true_pne, 1);
    disp(params);
    
    disp("======= pneumonia = false =======");
    params = sum(false_pne, 1)/size(false_pne, 1);
    disp(params);
    
end