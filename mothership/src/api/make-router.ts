import Router from 'express-promise-router';
import responseDecorators from './middleware/response-decorators';

export default function makeApiRouter() {
  const router = Router();

  router.use(responseDecorators);

  // Add routes here

  return router;
}
