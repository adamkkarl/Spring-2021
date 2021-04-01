function [calc] = main8_2inference
    symptoms = readmatrix("example.txt", 'FileType','text');
    size(symptoms)
    
    calc = [];
    
    gen_params = [0.606 0.504 0.116 0.506];
    pneu_true = [0.9 0.7 0.9 0.8];
    
    for j=1:size(symptoms,1)
        c = 0.02; %initialized to P(pneumonia=T)
        for i=1:4  
            if symptoms(j,i)~= -1 %do nothing if unknown
                if symptoms(j,i) == 1 %positive
                    c = c * pneu_true(1,i);
                    c = c / gen_params(1,i);
                else %negative
                    c = c * (1-pneu_true(1,i));
                    c = c / (1-gen_params(1,i));
                end
            end
        end
        calc = [calc; c]
    end
end