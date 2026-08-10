int lock_dongles(t_coder *coder)
{
    long long time_now;

    if (!coder || !coder->right_dongle || !coder->left_dongle)
        return (0);

    if (for_one_dongle(coder) == 0)
        return (0);

    check_first_and_second(coder);

    while (!get_stop(coder->data))
    {
        pthread_mutex_lock(&coder->first->mutex);

        time_now = time_current();
        if (time_now < coder->first->is_valid
            || coder->first->is_use != 0)
        {
            pthread_mutex_unlock(&coder->first->mutex);
            usleep(1000);
            continue;
        }

        pthread_mutex_lock(&coder->second->mutex);

        time_now = time_current();
        if (time_now >= coder->second->is_valid
            && coder->second->is_use == 0)
        {
            coder->first->is_use = 1;
            coder->second->is_use = 1;
            return (1);
        }

        pthread_mutex_unlock(&coder->second->mutex);
        pthread_mutex_unlock(&coder->first->mutex);
        usleep(1000);
    }

    return (0);
}