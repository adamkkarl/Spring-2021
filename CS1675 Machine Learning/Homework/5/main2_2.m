function main2_2()
    train = readmatrix("pima_train.txt");
    
    class_1 = [];
    class_0 = [];
    
    for i=1:size(train, 1)
        if train(i,9) == 1
            class_1 = [class_1; train(i,:)];
        else
            class_0 = [class_0; train(i,:)];
        end
    end
    
    disp("======CLASS 0 EXPFIT======");
    global class_0_muhat;
    class_0_muhat = [];
    for i=1:8
        
        [muhat, muci] = expfit(class_0(:,i));
        disp(i);
        disp(muhat);
        class_0_muhat = [class_0_muhat, muhat];
        disp(muci);
    end
    
    disp("======CLASS 1 EXPFIT======");
    global class_1_muhat;
    class_1_muhat = [];
    for i=1:8
        
        [muhat, muci] = expfit(class_1(:,i));
        disp(i);
        disp(muhat);
        class_1_muhat = [class_1_muhat, muhat];
        disp(muci);
    end
    
    
    disp("======CLASS 0 NORMFIT======");
    global class_0_mu;
    global class_0_sigma;
    class_0_mu = [];
    class_0_sigma = [];
    for i=1:8
        [mu, sigma, muci, sci] = normfit(class_0(:,i));
        disp(i);
        disp(mu);
        disp(sigma);
        class_0_mu = [class_0_mu, mu];
        class_0_sigma = [class_0_sigma, sigma];
        disp(muci);
        disp(sci);
    end
    
    disp("======CLASS 1 NORMFIT======");
    global class_1_mu;
    global class_1_sigma;
    class_1_mu = [];
    class_1_sigma = [];
    for i=1:8
        [mu, sigma, muci, sci] = normfit(class_1(:,i));
        disp(i);
        disp(mu);
        disp(sigma);
        class_1_mu = [class_1_mu, mu];
        class_1_sigma = [class_1_sigma, sigma];
        disp(muci);
        disp(sci);
    end
    
    disp(class_0_mu);
    disp(class_1_mu);

end