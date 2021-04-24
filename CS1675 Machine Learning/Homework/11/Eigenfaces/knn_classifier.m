function knn_classifier
    [train_img, train_label, test_img, test_label] = data_loader('data/classification');
    [eigenvecs, mean_matrix] = calc_eigen_vecs(train_img);
    
    for m=[5 20 50 100]
        for k=[1 5 10 20 40]
            disp("==================");
            disp(['m = ' num2str(m) ', k = ' num2str(k)]);

            low_dim_train = project_PCA(train_img,eigenvecs,mean_matrix,m);
            low_dim_test =  project_PCA(test_img, eigenvecs,mean_matrix,m);

            knn_model = fitcknn(low_dim_train.', train_label,'NumNeighbors',k);
            ypred_train = predict(knn_model, low_dim_train.');
            ypred_test  = predict(knn_model, low_dim_test.');

            %calc and display train misclassification error
            C_train = confusionmat(train_label, ypred_train);
            %disp(C_train); 
            correct_train = 0;
            for i=1:size(C_train,1)
               correct_train = correct_train + C_train(i,i);
            end
            mce_train = 1 - correct_train/size(ypred_train, 1);
            disp("Training data misclassification error:");
            disp(mce_train);


            %calc and display test misclassification error
            C_test = confusionmat(test_label, ypred_test);
            %disp(C_test); 
            correct_test = 0;
            for i=1:size(C_test,1)
               correct_test = correct_test + C_test(i,i);
            end
            mce_test = 1 - correct_test/size(ypred_test, 1);
            disp("Testing data misclassification error:");
            disp(mce_test);

        end
    end
end