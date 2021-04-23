function [S] = main_2_2b(train_img, train_label, test_img, test_label, eigenvecs, mean_matrix, img_num, m)
    low_dim_train = project_PCA(train_img,eigenvecs,mean_matrix,m);
    recon_train = reconstruct_PCA(low_dim_train, eigenvecs, mean_matrix);

    my_img = low_dim_train(:,img_num);
    
    match_img_num = -1;
    best_euclid = -1;
    
    for i=1:size(low_dim_train, 2)
        if i ~= img_num %don't want to match with self
            test_img = low_dim_train(:,i);
            
            euclid_dist = sqrt(sum((my_img-test_img).^2));
            if best_euclid < 1 || euclid_dist < best_euclid
               match_img_num = i;
               best_euclid = euclid_dist;
            end
        end
    end
    disp(match_img_num);
    disp(best_euclid);
    show_multiple_images([train_img(:,img_num), recon_train(:,img_num),train_img(:,match_img_num), recon_train(:,match_img_num)]);

end