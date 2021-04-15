function [train_E, test_E] = main_c(tr_x, tr_y, test_x, test_y)
    sum_train_E = 0;
    sum_test_E = 0;
    for i=1:20
        [pred_y, E] = Bag_classifier(tr_x,tr_y,test_x,'[@DT_base_simple,10,[]]');
        
        %error for training data
        sum_train_E = sum_train_E + E;
        
        %error for testing data
        C = confusionmat(logical(test_y), pred_y);
        test_class_error = (C(1,2) + C(2,1))/70;
        sum_test_E = sum_test_E + test_class_error;
    end
    
    %divide by 20 iterations
    train_E = sum_train_E/20;
    train_E = mean(train_E);
    test_E = sum_test_E/20;
end